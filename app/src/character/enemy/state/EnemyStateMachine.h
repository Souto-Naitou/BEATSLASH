#pragma once

#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <unordered_map>
#include <memory>

class EnemyStateMachine
{
public:
	// 状態の初期化
	void Initialize(std::initializer_list<EnemyStateType> stateTypes, Enemy* enemy);

	// 状態の更新
	void Update();

	// 状態の切り替え
	void ChangeState(EnemyStateType newStateType);

	// 現在の状態の取得
	EnemyStateType GetCurrentState() const { return currentStateType_; }

private:
	// 状態の管理
	std::unordered_map<EnemyStateType, std::unique_ptr<EnemyState>> states_;

	// 現在の状態
	EnemyStateType currentStateType_;

	// オーナーのポインタ
	Enemy* owner_;
};