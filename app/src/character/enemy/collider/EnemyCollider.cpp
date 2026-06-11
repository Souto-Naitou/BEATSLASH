#include "EnemyCollider.h"
#include <utility/CollisionUtility.h>
#include <type/ColliderTypeID.h>
#include <component/HPComponent.h>
#include <EmitterManager.h>
#include <character/enemy/Enemy.h>

uint32_t EnemyCollider::effectCount_ = 0;

EnemyCollider::EnemyCollider(Tako::EmitterManager* emitterManager)
    : pEmitterManager_(emitterManager)
{
	if (pEmitterManager_)
	{
        // 攻撃ヒットエフェクトのロード
        pEmitterManager_->LoadPreset("enemy_attack_hit");
        pEmitterManager_->SetEmitterActive("enemy_attack_hit", false); // 最初は非アクティブにしておく
	}
}

void EnemyCollider::OnCollisionEnter(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
    if (otherID == ColliderTypeID::Terrain || otherID == ColliderTypeID::Enemy)
    {
        // 当たったら一旦押し戻しを行う
        Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);
        // エネミーに押し戻しのコールバックが設定されていれば呼び出す
        if (pushBackCallback_)
        {
            pushBackCallback_(pushback);
        }
    }
    else if (otherID == ColliderTypeID::PlayerAttack)
    {
        //if (pHp_){
        //    pHp_->Damage(25); // TODO : 仮のダメージ値
        //}

		// 色を白くしてヒットをわかりやすくする
		if (auto owner = GetOwner())
		{
			auto enemy = static_cast<Enemy*>(owner);
			enemy->GetModel()->SetMaterialColor(enemy->GetModel()->GetMaterialColor() + color);
			enemy->GetModel()->Update();
		}

		// ヒットエフェクトの再生
		if (pEmitterManager_)
        {
			const std::string effectName = "enemy_attack_hit_" + std::to_string(effectCount_++);
			pEmitterManager_->CreateTemporaryEmitterFrom("enemy_attack_hit", effectName, 0.1f);
			pEmitterManager_->SetEmitterPosition(effectName, GetCenter());
        }
    }
}

void EnemyCollider::OnCollisionStay(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
    if (otherID == ColliderTypeID::Terrain || otherID == ColliderTypeID::Enemy)
    {
        // 当たったら一旦押し戻しを行う
        Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);
        // エネミーに押し戻しのコールバックが設定されていれば呼び出す
        if (pushBackCallback_)
        {
            pushBackCallback_(pushback);
        }
    }
    else if (otherID == ColliderTypeID::PlayerAttack)
    {
        // 色を戻してヒットが終わったことをわかりやすくする
        if (auto owner = GetOwner())
        {
            auto enemy = static_cast<Enemy*>(owner);
            enemy->GetModel()->SetMaterialColor({ 1.0f, 0.0f, 0.0f, 1.0f });
            enemy->GetModel()->Update();
        }
    }
}

void EnemyCollider::OnCollisionExit(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());

    if (otherID == ColliderTypeID::PlayerAttack)
    {
        // 色を強制的に戻しておく。(色が戻らないのを防ぐ)
        if (auto owner = GetOwner())
        {
            auto enemy = static_cast<Enemy*>(owner);
            enemy->GetModel()->SetMaterialColor({ 1.0f, 0.0f, 0.0f, 1.0f });
            enemy->GetModel()->Update();
        }
    }
}