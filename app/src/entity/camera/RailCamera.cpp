#include "RailCamera.h"



void RailCamera::Initialize(const Tako::Vector3* pCenter)
{
    pCamera_ = std::make_unique<Tako::Camera>();
    pCamera_->SetFarClip(1000.0f);
    pFocusOrientation_ = std::make_unique<FocusOrientation>();
    pFocusOrientation_->SetTargetPosition(pCenter);
}

void RailCamera::Update(AnimationTimeline<Tako::Vector3>& rail, float deltaTime)
{
    auto& position = rail.Update();
    pCamera_->SetTranslate(position);
    pCamera_->Update();
    Tako::Transform transform = pCamera_->GetTransform();
    pFocusOrientation_->Update(transform, deltaTime);
    kRotate_ = transform.rotate;
    pCamera_->SetRotate(kRotate_);
}
