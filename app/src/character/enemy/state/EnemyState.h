#pragma once
#include <optional>
#include <character/enemy/state/EnemyStateType.h>

class Enemy;

class EnemyState
{
public:
    virtual ~EnemyState() = default;

	// 状態遷移のチェック。遷移する場合は遷移先の状態を返す
	virtual std::optional<EnemyStateType> CheckTransition(Enemy* enemy) { return std::nullopt; }
	// 状態に入るときの処理
    virtual void Enter(Enemy* enemy) {}
	// 状態の更新処理
    virtual void Update(Enemy* enemy) {}
	// 状態から抜けるときの処理
    virtual void Exit(Enemy* enemy) {}
	// ImGuiの描画
	virtual void DrawImGui(Enemy* enemy) {}
};