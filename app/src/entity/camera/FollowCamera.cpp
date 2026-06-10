#include "FollowCamera.h"

#include "Object3dBasic.h"
#include <Draw2D.h>
#include <GPUParticle.h>

#include <Windows.h>
#include <utility/wndutl.h>
#include <WinApp.h>
#include <algorithm>

void FollowCamera::Initialize()
{
#ifdef _DEBUG
    shiftDirection_.SetOnChange([&](const Tako::Vector3&)
    {
        shiftDirection_ = shiftDirection_->Normalize();
    });
#else
    isActive_ = true; // デバッグモード以外では常にカメラ制御を有効にする
#endif // _DEBUG

    pCamera_ = std::make_unique<Tako::Camera>();

    pCameraInput_ = std::make_unique<CameraInput>();


    pCamera_->SetFarClip(1000.0f);
}

void FollowCamera::Update()
{
#ifdef _DEBUG
    translation_ = pCamera_->GetTranslate();
    rotation_ = pCamera_->GetRotate();
#endif // _DEBUG

    pCameraInput_->Update();

    const auto& command = pCameraInput_->GetCommand();

    this->CameraActivationUpdate(command);
    this->CursorFixUpdate();

    if (isActive_) this->CameraDataUpdate(pCameraInput_->GetCommand());
    else this->CameraDataUpdate({});
    pCamera_->Update();
}

void FollowCamera::CameraActivationUpdate(const CameraInput::Command& command)
{
#ifdef _DEBUG
    if (command.isCameraActivationTriggered) isActive_ = !isActive_;
#endif // _DEBUG


    if (!pCursorHidden_ && isActive_)
    {
        pCursorHidden_ = std::make_unique<ScopedCursorHidden>();
    }
    else if (pCursorHidden_ && !isActive_)
    {
        pCursorHidden_.reset();
    }
}

void FollowCamera::CameraDataUpdate(const CameraInput::Command& command)
{
    if (!pTarget_) return;

    // カメラ制御のトリガーがあるときはカメラの有効/無効を切り替えるだけで、カメラの回転や位置の更新は行わない
    // カーソルを固定するタイミングでカメラの回転や位置の更新も行うと、カーソルが固定された瞬間にカメラが大きく動いてしまうため
    if (command.isCameraActivationTriggered) return;

    // 入力を反映
    kRotation_->x += command.delta.pitch;
    kRotation_->y += command.delta.yaw;

    // ピッチを制限する
    this->PitchClamp(kRotation_->x);

    // 方向を計算
    Tako::Vector3 rotate = { kRotation_->x, kRotation_->y, 0.0f };
    Tako::Matrix4x4 rotMatrix = Tako::Mat4x4::MakeRotateXYZ(rotate);
    Tako::Vector3 direction = Tako::Mat4x4::TransformNormal(rotMatrix, shiftDirection_);

    // 補間
    Tako::Vector3 targetPosition = pTarget_->translate + targetPositionOffset_;
    Tako::Vector3 lerpedPosition = targetPositionPre_ * (1.0f - kFactorLerp_) + targetPosition * kFactorLerp_;

    Tako::Vector3 nextCameraPosition = direction.Normalize() * kOffset_ + lerpedPosition;

    pCamera_->SetRotate(rotate);
    pCamera_->SetTranslate(nextCameraPosition);

    targetPositionPre_ = lerpedPosition;
}

void FollowCamera::CursorFixUpdate()
{
    if (!pCursorHidden_ || !isActive_) return;

    auto center = utl::window::GetCenterOfWindow();

    ClientToScreen(Tako::WinApp::GetInstance()->GetHWnd(), &center);
    SetCursorPos(center.x, center.y);
}

void FollowCamera::PitchClamp(float& pitch)
{
    pitch = std::clamp(pitch, kMinPitch_.Get(), kMaxPitch_.Get());
}
