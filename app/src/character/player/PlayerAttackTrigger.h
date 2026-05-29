#pragma once
#include "PlayerInput.h"

/// 役割
/// - 入力を監視する
/// - クールタイムの管理
/// - コンボのカウント
/// - コライダーの生成要求 (boolean returning)
/// 呼び出しが必要な関数
/// - UpdateCooldown(float deltaTime): クールタイムの更新
class PlayerAttackTrigger
{
public:
    void UpdateCooldown(float deltaTime);
    bool ShouldAttack(const PlayerInput::PlayerCommand& command);
    void CalculateCooldownTime(float secPerBeat) { cooldownTime_ = secPerBeat * kNumBeatsPerAttack_; } // BPMに基づいてクールタイムを計算

private:
    float remainingCooldownTime_ = 0.0f; // 攻撃のクールタイムの残り時間 (秒)

    // パラメータ
    const float kNumBeatsPerAttack_ = 1.0f; // 1回の攻撃に必要な拍数
    float cooldownTime_ = 1.0f; // 攻撃のクールタイム（秒)
};