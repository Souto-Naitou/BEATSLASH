#pragma once
#include <character/boss/bt/BossBTActionBase.h>
#include <OBBCollider.h>
#include <Object3d.h>
#include <Transform.h>
#include <Vector3.h>
#include <memory>
#include <string>
#include <cstdint>

namespace Tako { class EmitterManager; }

/// <summary>
/// ボスの近接横振り攻撃ノード
/// ボスの横（正面から振り半角ぶん右）に細長い棒を出し、growIntervalBeats_拍ごとに長さを伸ばす
/// growCount_回伸びたら向きを固定して前方へ横振りする（当たり判定は振り中のみ有効）
/// BeatClock不在時は拍数を秒として扱う
/// </summary>
class BTMeleeAttackAction : public BossBTActionBase
{
public:
    BTMeleeAttackAction();
    ~BTMeleeAttackAction() override;

    Tako::BTNodeStatus Execute(Tako::BTBlackboard* blackboard) override;
    void Reset() override;
    void ApplyParameters(const nlohmann::json& params) override;
    nlohmann::json ExtractParameters() const override;

#ifdef _DEBUG
    bool DrawImGui() override;
#endif

protected:
    void OnStart(Tako::BTBlackboard* blackboard) override;

private:
    // 実行フェーズ
    enum class Phase
    {
        Growing,    // 棒の成長（予告）中
        Swinging,   // 横振り中
    };

    void UpdateBarPlacement(const Tako::Vector3& bossPos, float angle);
    void StartSwing(float baseYaw, const BeatClock* beatClock);
    void RemoveBarCollider();

private:
    // コライダーをモデル見た目より大きめにする倍率（エネミー攻撃準拠）
    static constexpr float kColliderSizeMultiplier = 3.4f;
    // 攻撃エフェクトのプリセット名
    static constexpr const char* kEffectPresetName = "enemy_attack";

    // 何拍ごとに棒が伸びるか
    float growIntervalBeats_ = 1.0f;
    // 何回伸びたら振るか
    int growCount_ = 3;
    // 横振りの長さ（拍）
    float swingDurationBeats_ = 2.0f;
    // 棒の初期長さ（モデルスケールX）
    float barStartLength_ = 4.0f;
    // 1成長あたりの伸び
    float lengthPerGrow_ = 2.0f;
    // 棒の厚み（高さ）
    float barHeight_ = 0.3f;
    // 棒の厚み（奥行き）
    float barDepth_ = 0.6f;
    // ボス中心から棒中心までの距離
    float offsetDistance_ = 10.0f;
    // 棒のY位置オフセット（ボス中心基準）
    float barOffsetY_ = 0.0f;
    // 振り半角（度）。正面+半角から-半角まで振る
    float swingHalfAngleDeg_ = 60.0f;
    // 成長SEの音量
    float growSeVolume_ = 0.5f;

    Phase phase_ = Phase::Growing;
    // 実行済み成長回数
    int performedGrows_ = 0;
    // 現在の棒の長さ
    float currentLength_ = 0.0f;
    // 振り中の固定向き
    float swingBaseYaw_ = 0.0f;
    // BeatClock不在時のフォールバック用経過秒数
    float elapsedSeconds_ = 0.0f;

    // 棒の見た目モデル（描画はBoss::QueueAttachedModelDraw経由で行う）
    std::unique_ptr<Tako::Object3d> pBarModel_;
    // 棒のトランスフォーム（コライダーとポインタ共有）
    Tako::Transform barTransform_;
    // 横振りの当たり判定
    std::unique_ptr<Tako::OBBCollider> pBarCollider_;
    bool colliderRegistered_ = false;

    // エフェクト削除用キャッシュ（所有しない。GameSceneのEmitterManagerはBTツリーより後に破棄される）
    Tako::EmitterManager* pEmitterManager_ = nullptr;
    // 棒モデルにバインドしたエフェクトテンプレートの一意名
    std::string effectBaseName_;
    // 直近に再生した一時エフェクト名
    std::string effectTempName_;
    // 一時エフェクト名の一意化カウンタ
    uint32_t playCount_ = 0;

    // インスタンスごとに一意なエフェクト名を作るための静的カウンタ
    static uint32_t sInstanceCounter_;
};
