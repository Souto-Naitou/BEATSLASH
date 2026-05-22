#include "FollowCamera.h"

#include "Object3dBasic.h"
#include <Draw2D.h>
#include <GPUParticle.h>

#include <Windows.h>
#include <utility/wndutl.h>
#include <WinApp.h>

void FollowCamera::Initialize()
{
#ifdef _DEBUG
    shiftDirection_.SetOnChange([&](const Tako::Vector3&)
    {
        shiftDirection_ = shiftDirection_->Normalize();
    });
#endif // _DEBUG

    pCamera_ = std::make_unique<Tako::Camera>();

    pCameraInput_ = std::make_unique<CameraInput>();

    Tako::Object3dBasic::GetInstance()->SetCamera(pCamera_.get());
    Tako::GPUParticle::GetInstance()->SetCamera(pCamera_.get());
    Tako::Draw2D::GetInstance()->SetCamera(pCamera_.get());

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
    this->CursorFixUpdate(command);

    if (isActive_) this->CameraDataUpdate(pCameraInput_->GetCommand());
    else this->CameraDataUpdate({});
    pCamera_->Update();
}

void FollowCamera::CameraActivationUpdate(const CameraInput::Command& command)
{
    if (command.isCameraActivationTriggered) isActive_ = !isActive_;

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

    // direction_
    Tako::Vector3 rotate = { kRotation_->x, kRotation_->y, 0.0f };
    Tako::Matrix4x4 rotation = Tako::Mat4x4::MakeRotateXYZ(rotate);
    Tako::Vector3 direction = Tako::Mat4x4::TransformNormal(rotation, shiftDirection_);

    // interpolation
    Tako::Vector3 nextTargetPosition = targetPositionPre_ * (1.0f - kFactorLerp_) + (pTarget_->translate + targetPositionOffset_) * kFactorLerp_;

    Tako::Vector3 nextCameraPosition = direction.Normalize() * kOffset_ + nextTargetPosition;

    pCamera_->SetRotate(rotate);
    pCamera_->SetTranslate(nextCameraPosition);

    targetPositionPre_ = nextTargetPosition;
}

void FollowCamera::CursorFixUpdate(const CameraInput::Command& command)
{
    if (!pCursorHidden_ || !isActive_) return;

    auto center = utl::window::GetCenterOfWindow();

    ClientToScreen(Tako::WinApp::GetInstance()->GetHWnd(), &center);
    SetCursorPos(center.x, center.y);
}
