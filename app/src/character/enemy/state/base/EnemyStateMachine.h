#pragma once

#include <character/enemy/state/base/EnemyState.h>
#include <character/enemy/state/base/EnemyStateFactry.h>
#include <unordered_map>
#include <memory>

class EnemyStateMachine
{
public:
	// 状態の初期化
	void Initialize(std::initializer_list<EnemyStateType> stateTypes, Enemy* enemy);

	// 状態の更新
	void Update();

	void ChangeState(EnemyStateType newStateType);

private:
	// 状態の管理
	std::unordered_map<EnemyStateType, std::unique_ptr<EnemyState>> states_;

	// 現在の状態
	EnemyStateType currentStateType_;

	// オーナーのポインタ
	Enemy* owner_;
};