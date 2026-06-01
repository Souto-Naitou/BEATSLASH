#include "CameraDirector.h"
#include <Object3dBasic.h>
#include <GPUParticle.h>
#include <Draw2D.h>

void CameraDirector::Initialize()
{
    pFollowCamera_ = std::make_unique<FollowCamera>();
    pFollowCamera_->Initialize();

    pFocusCamera_ = std::make_unique<FocusCamera>();
    pFocusCamera_->Initialize();

    Activate(pFollowCamera_->GetCamera());
}

void CameraDirector::Update(float deltaTime)
{
    switch (state_)
    {
    case State::Follow:
        pFollowCamera_->Update();
        break;
    case State::Focusing:
        pFocusCamera_->Update(deltaTime);
        if (pFocusCamera_->IsFinished())
        {
            state_ = State::Waiting;    // 一度だけ遷移（毎フレーム発火しない）
            if (onFocusArrived_) onFocusArrived_();
        }
        break;
    case State::Waiting:
        // NotifyDoorOpenFinished() が呼ばれるまで静止
        break;
    case State::Returning:
        pFocusCamera_->Update(deltaTime);
        if (pFocusCamera_->IsFinished())
        {
            pFocusCamera_->End();
            state_ = State::Follow;
            Activate(pFollowCamera_->GetCamera());
        }
        break;
    default:
        break;
    }
}

void CameraDirector::SetFollowTarget(const Tako::Transform* target)
{
    pFollowCamera_->SetTarget(target);
}

void CameraDirector::StartFocus(const Tako::Transform& target, float duration)
{
    if (state_ != State::Follow)
        return;

    pFocusCamera_->Start(
        pFollowCamera_->GetCamera()->GetTranslate(),
        pFollowCamera_->GetCamera()->GetRotate(),
        target,
        duration);
    state_ = State::Focusing;
    Activate(pFocusCamera_->GetCamera());
}

void CameraDirector::NotifyDoorOpenFinished()
{
    if (state_ != State::Waiting)
        return;

    pFocusCamera_->StartReturn(
        pFollowCamera_->GetCamera()->GetTranslate(),
        pFollowCamera_->GetCamera()->GetRotate(),
        kReturnDuration_);
    state_ = State::Returning;
}

void CameraDirector::Activate(Tako::Camera* camera)
{
    Tako::Object3dBasic::GetInstance()->SetCamera(camera);
    Tako::GPUParticle::GetInstance()->SetCamera(camera);
    Tako::Draw2D::GetInstance()->SetCamera(camera);
}
