#pragma once
#include <physics/ColliderTimer.h>
#include <physics/ColliderRepository.h>
#include <character/player/PlayerAttackHitReceiver.h>
#include "PlayerAttackPresentation.h"

#include <Transform.h>
#include <Collider.h>
#include <Vector3.h>
#include <memory>

class ComboBuffSystem;

class PlayerAttack
{
public:
    struct InitData
    {
        // コライダーの保存場所
        ColliderRepository&     colliderRepository;
        // コンボバフシステム
        ComboBuffSystem&        comboBuffSystem;
        // 生成する中心座標
        const Tako::Vector3&    position;
        // エミッターマネージャー
        Tako::EmitterManager&   emitterManager;
    };

    PlayerAttack(InitData& initData);
    void Update(float deltaTime);
    bool IsActive() const { return pCollider_ != nullptr; }

private:
    std::unique_ptr<PlayerAttackHitReceiver>    pHitReceiver_;      // プレイヤーの攻撃ヒット受信クラス
    std::unique_ptr<PlayerAttackPresentation>   pPresentation_;     // 攻撃のエフェクト管理クラス

    float           kColliderActiveTime_    = 0.5f;     // コライダーの有効時間
    Tako::Transform transform_              = {};       // 攻撃のトランスフォーム（デバッグ表示用）
    Tako::Collider* pCollider_              = nullptr;  // 攻撃用コライダー
    ColliderTimer   colliderTimer_          = {};       // コライダーの有効時間管理
};
