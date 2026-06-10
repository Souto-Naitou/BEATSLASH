#pragma once
#include <character/enemy/state/EnemyState.h>
#include <Vector3.h>

namespace Tako
{
	class EmitterManager;
}

class EnemyDeadState : public EnemyState
{
public:
	EnemyDeadState(Tako::EmitterManager* emitterManager = nullptr);
	~EnemyDeadState() override = default;

	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override {}
	void Draw(Enemy* enemy) override {}
	void DrawImGui(Enemy* enemy) override {}

private:
	// エミッターマネージャーのポインタ。所有しない（GameSceneが所有し、GameSceneの寿命まで生きる）
	Tako::EmitterManager* pEmitterManager_ = nullptr;

	// 死亡演出のタイマー
	float timer_ = 0.0f;

	// 死亡演出（縮小）の持続時間（秒）
	static constexpr float kDeadDuration_ = 0.7f;

	// 初期スケール
	Tako::Vector3 startScale_ = { 1.0f, 1.0f, 1.0f };
};
