#pragma once
#include <OBBCollider.h>
#include <string>

namespace Tako
{
	class EmitterManager;
}

class EnemyAttackCollider : public Tako::OBBCollider
{
public:
	EnemyAttackCollider(Tako::EmitterManager* emitterManager);
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;
private:
	// エミッターマネージャーのポインタ。所有しない（GameSceneが所有し、GameSceneの寿命まで生きる）
	Tako::EmitterManager* pEmitterManager_ = nullptr;
	// エフェクトの名前
	std::string effectName_ = "enemy_attack_hit_effect";
	// エフェクトのカウント
	static uint32_t effectCount_;
};
