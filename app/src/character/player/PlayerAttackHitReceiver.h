#pragma once
#include <combo/ComboBuffSystem.h>
#include <presentation/animation/RadialBeat.h>
#include <entity/attack/PlayerAttackPresentation.h>
#include <debug\GameParameter.h>
#include <skill/Overdrive.h>
#include <EmitterManager.h>
#include <Vector3.h>

#include <memory>


/// <summary>
/// プレイヤーの攻撃ヒット受信クラス
/// Observerパターンとファサードパターンの組み合わせ
/// </summary>
class PlayerAttackHitReceiver
{
public:
    struct HitInfo
    {
        Tako::Vector3 position = {}; // ヒット位置
    };

    struct Executors
    {
        // コンボシステム
        ComboBuffSystem& comboBuffSystem;
        // オーバードライブ
        Overdrive& overdrive;
        PlayerAttackPresentation& playerAttackPresentation;
    };

    PlayerAttackHitReceiver(Executors& execs);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 攻撃ヒット受信
    /// </summary>
    /// <param name="info">ヒット情報</param>
    void ReceiveHit(const HitInfo& info);

private:
    EnableDebug("PlayerAttackHitReceiver");

    GameParameter(float, kTimeRadialBeat_, 0.3f);
    GameParameter(std::string, kEmitterName_, "player_attack_short");

    std::unique_ptr<RadialBeat> pRadialBeat_;
    Executors execs_;
    uint32_t emitterIndex_ = 0;
};