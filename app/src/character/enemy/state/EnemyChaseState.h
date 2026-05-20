#pragma once
#include "EnemyState.h"
#include <Vector3.h>
#include <Input.h>

class EnemyChaseState : public EnemyState
{
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;

private:
	// ターゲットの座標を更新する
	// NOTE:実際にはターゲットが敵なので移動はここでさせなくていいので後で消す
	void TargetUpdate();

private:
	// 仮のターゲット座標
	Tako::Vector3 targetPos_ = { 0.0f, 2.0f, 0.0f };
	// ターゲットを移動させるために使うInput
	Tako::Input* pInput_ = nullptr;

	// 追跡速度
	float chaseSpeed_ = 5.0f;
};

