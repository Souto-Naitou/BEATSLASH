#include "FollowCamera.h"

#include "Object3dBasic.h"
#include <Draw2D.h>
#include <GPUParticle.h>

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

    CameraUpdate(pCameraInput_->GetCommand());
}

void FollowCamera::CameraUpdate(const CameraInput::Command& command)
{
    if (!pTarget_) return;

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
    pCamera_->Update();

    targetPositionPre_ = nextTargetPosition;
}
