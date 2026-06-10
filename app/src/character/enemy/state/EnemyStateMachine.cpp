#include "EnemyStateMachine.h"
#include <character/enemy/state/EnemyStateFactry.h>
#ifdef _DEBUG
#include <DebugUIManager.h>
#endif


void EnemyStateMachine::Initialize(std::initializer_list<EnemyStateType> stateTypes, Enemy* enemy, const ICharacter* target)
{
	// 指定された状態に基づいて状態を初期化
	for (EnemyStateType type : stateTypes)
	{
		states_[type] = EnemyStateFactory::Create(type, target);
	}

	// 最初の状態を設定(受け取った状態リストの最初の要素を渡す)
	currentStateType_ = *stateTypes.begin();

	// オーナーのポインタを保存
	owner_ = enemy;

	// 最初の状態に入る
	if (states_.find(currentStateType_) != states_.end())
	{
		states_[currentStateType_]->Enter(owner_);
		// デバッグUIに登録
#ifdef _DEBUG
		Tako::DebugUIManager::GetInstance()->RegisterGameObject("Enemy State", [this]() { states_[currentStateType_]->DrawImGui(owner_); });
#endif
	}
}

void EnemyStateMachine::Update()
{
	// 現在の状態の更新
	if (states_.find(currentStateType_) != states_.end() && states_[currentStateType_] != nullptr)
	{
		states_[currentStateType_]->Update(owner_);
	}

	// 状態遷移のチェック
	if (states_.find(currentStateType_) != states_.end() && states_[currentStateType_] != nullptr)
	{
		std::optional<EnemyStateType> newStateType = states_[currentStateType_]->CheckTransition(owner_);
		if (newStateType.has_value())
		{
			ChangeState(newStateType.value());
		}
	}
}

void EnemyStateMachine::ChangeState(EnemyStateType newStateType)
{
	// すでに同じ状態の場合は早期リターン
	if (currentStateType_ == newStateType)
	{
		return; 
	}

	// 現在の状態から抜ける
	if (states_.find(currentStateType_) != states_.end() && states_[currentStateType_] != nullptr)
	{
		states_[currentStateType_]->Exit(owner_); // 状態から抜けるときの処理

		// デバッグUIの登録解除
#ifdef _DEBUG
		Tako::DebugUIManager::GetInstance()->UnregisterGameObject("Enemy State");
#endif
	}

	// 新しい状態に入る
	if (states_.find(newStateType) != states_.end() && states_[newStateType] != nullptr)
	{
		currentStateType_ = newStateType;
		states_[currentStateType_]->Enter(owner_); // 状態に入るときの処理

		// デバッグUIに登録
#ifdef _DEBUG
		Tako::DebugUIManager::GetInstance()->RegisterGameObject("Enemy State", [this]() { states_[currentStateType_]->DrawImGui(owner_); });
#endif
	}
}
