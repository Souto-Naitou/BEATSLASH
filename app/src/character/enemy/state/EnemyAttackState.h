#pragma once
#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <optional>
#include <memory>
#include "character/enemy/collider/EnemyAttackCollider.h"
#include <character/ICharacter.h>
#include <Object3d.h>
#include <array>
#include <string>

namespace Tako
{
	class EmitterManager;
}

enum class AttackEffectType
{
	Main,          // 攻撃中スイング
	Warning,       // 予備動作全体
	WarningWeapon, // 予備動作武器
	Count
};

struct AttackEffectInfo
{
	std::string presetName; // プリセット名 ("enemy_attack" など)
	std::string baseName;   // ロード用の一意な名前
	std::string tempName;   // 一時再生用の名前
};

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
	// エフェクト管理用配列
	std::array<AttackEffectInfo, static_cast<size_t>(AttackEffectType::Count)> effects_;
	// 一時的なエミッターの一意性を保証するためのローカルプレイカウント
	uint32_t playCount_ = 0;
	// インスタンスの一意性を保証するための静的IDカウンタ
	static uint32_t nextInstanceId_;
	// エミッターのボックスサイズ
	static constexpr Tako::Vector3 kEmitterBoxSize = { 5.0f, 0.2f, 0.7f };
	// 予備動作時間
	static constexpr float kWarningDuration_ = 0.4f;
	// 予備動作時間（拍数）
	static constexpr float kWarningDurationInBeats = 1.0f;
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
	// 攻撃が開始されたか
	bool isAttackStarted_ = false;
};

