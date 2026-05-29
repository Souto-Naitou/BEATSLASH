#include "FocusCamera.h"

void FocusCamera::Initialize()
{
    pCamera_ = std::make_unique<Tako::Camera>();
}

void FocusCamera::Update(float deltaTime)
{
    pCamera_->Update();
}

void FocusCamera::Start(const Tako::Vector3& fromPos, const Tako::Vector3& fromRot, const Tako::Transform& target, float duration)
{
    isActive_ = true;

    targetPos_ = target.translate;
    targetRot_ = target.rotate;

    startPos_ = fromPos;
    startRot_ = fromRot;

    duration_ = duration;
    elapsed_ = 0.0f;
}

void FocusCamera::End()
{
    isActive_ = false;
}
