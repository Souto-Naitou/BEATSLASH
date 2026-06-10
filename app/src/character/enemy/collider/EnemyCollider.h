#pragma once
#include <OBBCollider.h>
#include <functional>
#include <string>

namespace Tako
{
	class EmitterManager;
}

using EnemyPushBackCallback = std::function<void(const Tako::Vector3& pushback)>;

class HPComponent;

class EnemyCollider : public Tako::OBBCollider
{
public:
	EnemyCollider(Tako::EmitterManager* emitterManager);
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	void SetPushBackCallback(EnemyPushBackCallback callback) { pushBackCallback_ = callback; }
    void SetHPComponent(HPComponent* hp) { pHp_ = hp; }
private:
	EnemyPushBackCallback pushBackCallback_;

    HPComponent* pHp_ = nullptr;

	// エミッターマネージャーのポインタ。所有しない（GameSceneが所有し、GameSceneの寿命まで生きる）
	Tako::EmitterManager* pEmitterManager_ = nullptr;
	// エフェクトの名前
	std::string effectName_ = "enemy_attack_hit_effect";
	// エフェクトのカウント
	static uint32_t effectCount_;
};

