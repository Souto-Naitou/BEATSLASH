#pragma once
#include <Camera.h>
#include <memory>
#include <Transform.h>
#include <Vector3.h>
#include <debug/GameParameter.h>
#include "CameraInput.h"

class FollowCamera
{
public:
    void Initialize();
    void Update();

    void SetTarget(const Tako::Transform* pTarget) { pTarget_ = pTarget; }
    void SetTranslate(const Tako::Vector3& translate) { pCamera_->SetTranslate(translate); }
    Tako::Camera* GetCamera() { return pCamera_.get(); }

private:
    void CameraUpdate(const CameraInput::Command& command);

    EnableDebug("Follow Camera");

    std::unique_ptr <Tako::Camera> pCamera_ = nullptr;
    const Tako::Transform* pTarget_ = nullptr;

    /// Components
    std::unique_ptr<CameraInput> pCameraInput_ = nullptr;

    /// Parameters
    GameParameter(Tako::Vector3, shiftDirection_, Tako::Vector3({ 0.0f, 0.08f, -0.997f }));
    GameParameter(Tako::Vector3, targetPositionOffset_, Tako::Vector3({ 0.0f, 0.0f, 0.0f }));
    GameParameter(float, kOffset_, 35.0f);              // カメラとターゲットの距離
    GameParameter(float, kFactorLerp_, 0.01f);          // 線形補間の係数
    GameParameter(Tako::Vector2, kRotation_, {});     // カメラのX軸回転
    GameParameterView(Tako::Vector3, rotation_, {});    // ターゲットの位置（デバッグ表示用）
    GameParameterView(Tako::Vector3, translation_, {}); // ターゲットの位置（デバッグ表示用）

    Tako::Vector3 targetPositionPre_ = {};
};

