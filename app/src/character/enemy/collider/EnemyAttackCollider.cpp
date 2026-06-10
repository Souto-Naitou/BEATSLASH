#include "EnemyAttackCollider.h"
#include <type/ColliderTypeID.h>
#include <EmitterManager.h>
#include <Collider.h>

uint32_t EnemyAttackCollider::effectCount_ = 0;

EnemyAttackCollider::EnemyAttackCollider(Tako::EmitterManager* emitterManager)
	: pEmitterManager_(emitterManager)
{
	// 攻撃ヒットエフェクトのロード
	pEmitterManager_->LoadPreset("enemy_attack_hit");
	pEmitterManager_->SetEmitterActive("enemy_attack_hit", false); // 最初は非アクティブにしておく
}

void EnemyAttackCollider::OnCollisionEnter(Tako::Collider* other)
{
	ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
	if (otherID == ColliderTypeID::Player)
	{
		// ヒットエフェクトを出す
		effectName_ = "enemy_attack_hit_effect_" + std::to_string(effectCount_++);
		pEmitterManager_->CreateTemporaryEmitterFrom("enemy_attack_hit", effectName_, 0.1f);
		pEmitterManager_->SetEmitterPosition(effectName_, other->GetCenter());
	}
}

void EnemyAttackCollider::OnCollisionStay(Tako::Collider* other)
{

}

void EnemyAttackCollider::OnCollisionExit(Tako::Collider* other)
{

}