#pragma once
#include <memory>
#include <EmitterManager.h>
#include <Vector3.h>
#include <string>
#include <cstdint>
#include <unordered_map>

/// Player::Update() -> 
/// AttackRepository::CreatePlayerAttack() -> 
/// PlayerAttack::PlayerAttack() -> 
/// PlayerAttackPresentation::PlayerAttackPresentation()

/// <summary>
/// 攻撃のエフェクトを管理するクラス
/// </summary>
class PlayerAttackPresentation
{
public:
    PlayerAttackPresentation(Tako::EmitterManager& emitterManager);
    ~PlayerAttackPresentation();
    void Update();
    void OnHit();

    void SetColliderPositionRef(const Tako::Vector3* pPositionCollider) { pPositionCollider_ = pPositionCollider; }

private:
    // エミッターマネージャー
    Tako::EmitterManager&                               emitterManager_;
    // エミッターの一意な名前とインデックスのマップ
    static std::unordered_map<std::string, uint32_t>    emitterIndexMap_;
    // コライダーの位置参照
    const Tako::Vector3*                                pPositionCollider_ = nullptr;

    // 一意なトレイルの名前
    std::string uniqueNameTrail_;
    std::string uniqueNameTrailBB_;
};