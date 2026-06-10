#pragma once
#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <optional>
#include <memory>
#include "character/enemy/collider/EnemyAttackCollider.h"
#include <character/ICharacter.h>
#include <Object3d.h>

namespace Tako
{
	class EmitterManager;
}

class EnemyAttackState : public EnemyState
{
public:
	EnemyAttackState(const ICharacter* target = nullptr, Tako::EmitterManager* emitterManager = nullptr);
	~EnemyAttackState();
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;
	void Draw(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;
	std::optional<EnemyStateType> CheckTransition(Enemy* enemy) override;

private:
	// ターゲットのポインタ
	const ICharacter* pTarget_ = nullptr;
	// エミッターマネージャーのポインタ。所有しない（GameSceneが所有し、GameSceneの寿命まで生きる）
	Tako::EmitterManager* pEmitterManager_ = nullptr;
	// 攻撃のコライダー
	std::unique_ptr<EnemyAttackCollider> pAttackCollider_;
	// 攻撃エフェクトモデル
	std::unique_ptr<Tako::Object3d> pAttackModel_;
	// コライダーのトランスフォーム
	Tako::Transform colliderTransform_;
	// 攻撃エフェクトのカウント(ワンショット用)
	static uint32_t attackEffectCount_;
	// 攻撃エフェクトのモデルカウント(ワンショット用)
	static uint32_t attackEffectModelCount_;
	// エフェクトプリセット名のベース
	std::string effectName_;
	// エフェクトの一時的なエミッターの名前
	std::string emitterTempName_;
	// エミッターのボックスサイズ
	static constexpr Tako::Vector3 kEmitterBoxSize = { 5.0f, 0.2f, 0.7f };
	// 攻撃持続時間
	static constexpr float kAttackDuration_ = 0.6f;
	// 攻撃持続時間（拍数）
	static constexpr float kAttackDurationInBeats = 2.0f;
	// 攻撃コライダーの出現オフセット
	static constexpr float kColliderOffset = 6.0f;
	// 攻撃時間のタイマー
	float timer_ = 0.0f;
	// 攻撃開始時の拍数
	float startBeat_ = 0.0f;
	
};

