#pragma once
#include <Camera.h>
#include <memory>
#include <Transform.h>
#include <Vector3.h>
#include <debug/GameParameter.h>
#include "CameraInput.h"
#include <system/ScopedCursorHidden.h>
#include <Vector2.h>

class FollowCamera
{
public:
    void Initialize();
    void Update();

    void SetTarget(const Tako::Transform* pTarget) { pTarget_ = pTarget; }
    void SetTranslate(const Tako::Vector3& translate) { pCamera_->SetTranslate(translate); }

    Tako::Camera* GetCamera() { return pCamera_.get(); }
    Tako::Vector2 GetRotation() const { return { pCamera_->GetRotateX(), pCamera_->GetRotateY() }; }

private:
    void CameraActivationUpdate(const CameraInput::Command& command);
    void CameraDataUpdate(const CameraInput::Command& command);
    void CursorFixUpdate();
    void PitchClamp(float& pitch);

    EnableDebug("Follow Camera");

    const Tako::Transform* pTarget_ = nullptr;

    /// Components
    std::unique_ptr <Tako::Camera> pCamera_ = nullptr;
    std::unique_ptr<CameraInput> pCameraInput_ = nullptr;
    std::unique_ptr<ScopedCursorHidden> pCursorHidden_ = nullptr;

    /// Parameters
    GameParameter(Tako::Vector3, shiftDirection_, Tako::Vector3({ 0.0f, 0.08f, -0.997f }));
    GameParameter(Tako::Vector3, targetPositionOffset_, Tako::Vector3({ 0.0f, 0.0f, 0.0f }));
    GameParameter(float, kOffset_, 20.0f);              // カメラとターゲットの距離
    GameParameter(float, kFactorLerp_, 0.1f);           // 線形補間の係数
    GameParameter(Tako::Vector2, kRotation_, {});       // カメラのX軸回転
    GameParameter(bool, isActive_, false);              // カメラ制御の有効フラグ
    GameParameter(float, kMinPitch_, 0.0f);             // カメラの最小ピッチ（下方向の回転制限）
    GameParameter(float, kMaxPitch_, 1.57f);            // カメラの最大ピッチ（上方向の回転制限）
    GameParameterView(Tako::Vector3, rotation_, {});    // ターゲットの位置（デバッグ表示用）
    GameParameterView(Tako::Vector3, translation_, {}); // ターゲットの位置（デバッグ表示用）

    Tako::Vector3 targetPositionPre_ = {};
};

