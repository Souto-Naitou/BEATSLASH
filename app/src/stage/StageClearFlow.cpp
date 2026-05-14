#include "StageClearFlow.h"
#include <Logger.h>
#include <CollisionManager.h>
#include <imgui.h>
#include <format>
#include <SceneManager.h>

void StageClearFlow::Initialize()
{
	transitionCollider_ = std::make_unique<StageTransitionCollider>();
    transitionCollider_->SetOnTransitionTrigger([this]() { OnColliderHit(); });
    transitionCollider_->SetTypeID(100); // 適当な型IDを設定 TODO : enum 
	Tako::CollisionManager::GetInstance()->AddCollider(transitionCollider_.get());
	Tako::CollisionManager::GetInstance()->SetCollisionMask(1, 100, true);

}

void StageClearFlow::Update(float deltaTime)
{
	switch (state_)
	{
		case StageClearFlow::StageFlowState::Playing:
            // 特に更新処理はないはず
			break;
		case StageClearFlow::StageFlowState::StageClear:
            ClearUpdate(deltaTime);
			break;
		case StageClearFlow::StageFlowState::Transitioning:
            TransitionUpdate(deltaTime);
			break;
		default:
			break;
	}
}

void StageClearFlow::NotifyClear()
{
    ClearEnter();
}

void StageClearFlow::ClearEnter()
{
    state_ = StageFlowState::StageClear;
}

void StageClearFlow::TransitionEnter()
{
    state_ = StageFlowState::Transitioning;
}

void StageClearFlow::ClearUpdate(float deltaTime)
{
    // トランジションへの遷移はコライダーがイベントを受け取ったときなどの条件で行う想定
}

void StageClearFlow::TransitionUpdate(float deltaTime)
{
	if(true)// 遷移完了条件
	{
		state_ = StageFlowState::Playing;
	}
}

void StageClearFlow::ClearExit()
{
}

void StageClearFlow::TransitionExit()
{
}

void StageClearFlow::OnColliderHit()
{
    TransitionEnter();
    Tako::Logger::Log("Transition triggered by collider hit!");
}

