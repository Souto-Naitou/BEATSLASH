#include "FocusCamera.h"

#include <cmath>
#include <algorithm>
#include <numbers>

namespace
{
    // 最短角度差に丸め込む (-π ～ π)
    float WrapPi(float a)
    {
        constexpr float pi = std::numbers::pi_v<float>;
        while (a >  pi) a -= 2.0f * pi;
        while (a < -pi) a += 2.0f * pi;
        return a;
    }

    // 角度を最短経路でLerp
    float LerpAngle(float from, float to, float t)
    {
        return from + WrapPi(to - from) * t;
    }

    // SmoothStep (3t² - 2t³)
    float SmoothStep(float t)
    {
        t = std::clamp(t, 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }
}

void FocusCamera::Initialize()
{
    pCamera_ = std::make_unique<Tako::Camera>();
    pCamera_->SetFarClip(1000.0f);
}

void FocusCamera::Update(float deltaTime)
{
    if (!isActive_) return;

    elapsed_ += deltaTime;
    float smooth = SmoothStep(elapsed_ / duration_);

    // 位置補間
    Tako::Vector3 pos =
    {
        startPos_.x + (targetPos_.x - startPos_.x) * smooth,
        startPos_.y + (targetPos_.y - startPos_.y) * smooth,
        startPos_.z + (targetPos_.z - startPos_.z) * smooth,
    };

    // 回転補間（最短角度差）
    Tako::Vector3 rot =
    {
        LerpAngle(startRot_.x, targetRot_.x, smooth),
        LerpAngle(startRot_.y, targetRot_.y, smooth),
        LerpAngle(startRot_.z, targetRot_.z, smooth),
    };

    pCamera_->SetTranslate(pos);
    pCamera_->SetRotate(rot);
    pCamera_->Update();
}

void FocusCamera::Start(const Tako::Vector3& fromPos, const Tako::Vector3& fromRot, const Tako::Transform& target, float duration)
{
    isActive_ = true;
    startPos_ = fromPos;
    startRot_ = fromRot;
    duration_ = duration;
    elapsed_  = 0.0f;

    // 扉の forward ベクトルを回転行列から取得
    Tako::Matrix4x4 rotMatrix = Tako::Mat4x4::MakeRotateXYZ(target.rotate);
    const Tako::Vector3 doorDefaultForward = { 0.0f, 0.0f, -1.0f }; // モデルの正面方向（Z-）
    Tako::Vector3 doorForward = Tako::Mat4x4::TransformNormal(rotMatrix, doorDefaultForward);

    // 見下ろし角（度→ラジアン）から高さを算出
    constexpr float kDeg2Rad = std::numbers::pi_v<float> / 180.0f;
    float vertical = kFocusDistance_ * std::tanf(kElevationAngle_ * kDeg2Rad);

    targetPos_ = target.translate
               + doorForward * kFocusDistance_
               + Tako::Vector3{ 0.0f, vertical, 0.0f };

    // 目標位置 → 扉 を向く pitch/yaw を算出
    Tako::Vector3 dir = target.translate + targetOffset_ - targetPos_;
    float len = dir.Length();
    if (len > 0.0001f)
    {
        targetRot_.y =  std::atan2f(dir.x, dir.z);    // Yaw
        targetRot_.x = -std::asinf(dir.y / len);       // Pitch
        targetRot_.z =  0.0f;
    }
    else
    {
        targetRot_ = fromRot;
    }
}

void FocusCamera::StartReturn(const Tako::Vector3& toPos, const Tako::Vector3& toRot, float duration)
{
    startPos_ = pCamera_->GetTranslate();
    startRot_ = pCamera_->GetRotate();
    targetPos_ = toPos;
    targetRot_ = toRot;
    duration_  = duration;
    elapsed_   = 0.0f;
    isActive_  = true;
}

void FocusCamera::End()
{
    isActive_ = false;
}
