#pragma once
#include <BTNode.h>
#include <Transform.h>
#include <Vector3.h>
#include <physics/ColliderTimer.h>
#include <memory>
#include <cstdint>

class BossRazerCollider;
class BeatClock;
namespace Tako { class EmitterManager; }

/// <summary>
/// チャージ後にレーザーを発射するボス攻撃ノード
/// chargeIntervalBeats_拍ごとにチャージ球を拡大し、chargeCount_回チャージしたらターゲットへ発射する
/// BeatClock不在時はchargeIntervalBeats_を秒として扱う
/// </summary>
class BTChargeRazer : public Tako::BTNode
{
public:
    BTChargeRazer();
    ~BTChargeRazer() override;

    Tako::BTNodeStatus Execute(Tako::BTBlackboard* blackboard) override;
    void Reset() override;
    void ApplyParameters(const nlohmann::json& params) override;
    nlohmann::json ExtractParameters() const override;

#ifdef _DEBUG
    bool DrawImGui() override;
#endif

private:
    // 実行フェーズ
    enum class Phase
    {
        Charging,   // チャージ中
        Firing,     // レーザー発射中
    };

    void StartCharging(const Tako::Vector3& ballPos, const BeatClock* beatClock);
    void ApplyChargeStep();
    void Fire(const Tako::Vector3& ballPos, const Tako::Vector3& targetPos);
    void DeactivateChargeEffects();
    void RemoveRazerCollider();

private:
    // 距離ゼロ除算回避用の最小発射距離
    static constexpr float kMinFireDistance = 0.01f;

    // チャージ間隔（拍。BeatClock不在時は秒として扱う）
    float chargeIntervalBeats_ = 2.0f;
    // 発射までのチャージ回数
    int chargeCount_ = 3;
    // 1チャージあたりの半径増加量
    float radiusPerCharge_ = 0.5f;
    // チャージ球のボス基準オフセット
    Tako::Vector3 ballOffset_ = { 0.0f, 4.0f, 0.0f };
    // チャージ開始時の球の半径
    float ballStartRadius_ = 0.1f;
    // レーザー（エミッター・コライダー）の生存秒数
    float fireDuration_ = 0.1f;
    // レーザー断面（プリセットのboxSize.x/y）に掛ける倍率
    float laserThicknessScale_ = 1.0f;
    // ターゲット位置を超えて伸ばす追加長
    float laserExtraLength_ = 0.0f;
    // 狙い位置のYオフセット（ターゲット原点が足元の場合の補正用）
    float aimOffsetY_ = 0.0f;

    Phase phase_ = Phase::Charging;
    bool isStarted_ = false;
    // 実行開始時の拍
    float startBeat_ = 0.0f;
    // BeatClock不在時のフォールバック用経過秒数
    float elapsedSeconds_ = 0.0f;
    // 実行済みチャージ回数
    int performedCharges_ = 0;

    // Resetでのクリーンアップ用キャッシュ（所有しない。デストラクタでは破棄順の都合で使用禁止）
    Tako::EmitterManager* pEmitterManager_ = nullptr;
    // レーザーの当たり判定
    std::unique_ptr<BossRazerCollider> pRazerCollider_;
    // コライダーとポインタ共有するトランスフォーム
    Tako::Transform colliderTransform_;
    bool colliderRegistered_ = false;
    // 発射フェーズの有効時間管理
    ColliderTimer colliderTimer_;

    // 一時trailエミッターの一意名カウンタ
    static uint32_t sTrailCounter_;
};
