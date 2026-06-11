#pragma once

#include <debug/GameParameter.h>

#include <character/ICharacter.h>

#include "PlayerInput.h"
#include "PlayerMovement.h"
#include "PlayerAttackTrigger.h"
#include "ParryHistory.h"
#include "ParryJudgement.h"
#include "ParryPresentation.h"

#include <entity/camera/FollowCamera.h>
#include <component/collider/PlayerCollider.h>
#include <entity/attack/AttackRepository.h>
#include <manager/BeatManager.h>
#include <skill/Overdrive.h>
#include <skill/UpTempo.h>
#include <component/StateMachine.hpp>
#include <component/HPComponent.h>

#include <Object3d.h>
#include <Transform.h>
#include <memory>
#include <array>
#include <EmitterManager.h>

// 前方宣言
class ComboBuffSystem;
struct PlayerStateContext;

class Player : public ICharacter
{
public:
    enum class State
    {
        Idle,
        kSize,
    };
    static constexpr size_t kNumStates = static_cast<size_t>(State::kSize);

    struct InitData
    {
        AttackRepository& attackRepository;
        FollowCamera& followCamera;
        ComboBuffSystem& comboBuffSystem;
        BeatClock& beatClock;
        Tako::EmitterManager& emitterManager_;
    };

    using IPlayerState = IState<PlayerStateContext>;

    Player(const InitData& initData) : 
        attackRepository_(initData.attackRepository), 
        followCamera_(initData.followCamera),
        comboBuffSystem_(initData.comboBuffSystem),
        beatClock_(initData.beatClock),
        particleEmitter_(initData.emitterManager_)
    {}

    void Initialize() override;
    void Finalize();
    void Update() override;
    void Draw() override;
    void RegisterCallbacks();

    const Tako::Vector3& GetPosition()      const           { return transform_.translate; }
    const Tako::Vector3& GetScale()         const override  { return transform_.scale; }
    const Tako::Vector3& GetRotation()      const override  { return  transform_.rotate; }
    const Tako::Transform& GetTransform()   const           { return transform_; }
    Tako::Transform& GetTransform()                         { return transform_; }

    const HPComponent& GetHPComponent()   const { return *pHPComponent_; }
    const PlayerInput& GetPlayerInput()   const { return *pInput_; }

    void Respawn(const Tako::Transform& spawnTransform);

private:
    void InitializeComponents();
    void InitializeStates();
    void InitializeSkills();
    void InitializeObject3d();
    void InitializeCollider();
    void UpdateSkills(const PlayerInput::PlayerCommand& inputCommand);

    EnableDebug("Player");

    /// パラメータ
    GameParameter(float, kFrictionPower_, 0.8f);                                    // 摩擦係数
    GameParameter(float, kMovePower_, 180.0f);                                      // 移動力
    GameParameter(float, kMovePowerOnAttack_, 80.0f);                               // 移動力
    GameParameter(float, kJumpPower_, 5.0f);                                        // ジャンプ力
    GameParameter(float, kMass_, 60.0f);                                            // 重力
    GameParameter(float, kAnimationSpeed_, 3.5f);                                   // アニメーションの速度
    GameParameter(Tako::Vector3, kColliderSize_, Tako::Vector3(1.0f, 2.0f, 1.0f));  // コライダーのサイズ
    GameParameter(float, kDamagedEffectDuration_, 0.1f);                            // 被弾エフェクトの生存秒数

    uint32_t countAttack_ = 0; // 攻撃のカウント

    /// インスタンス
    std::unique_ptr<PlayerInput>                        pInput_;                    // プレイヤー入力管理クラス
    std::unique_ptr<PlayerMovement>                     pMovement_;                 // プレイヤー移動処理クラス
    std::unique_ptr<Tako::Object3d>                     pModel_;                    // キャラクターの3Dモデル
    std::unique_ptr<PlayerAttackTrigger>                pAttackTrigger_;            // プレイヤーの攻撃トリガー
    std::unique_ptr<PlayerCollider>                     pCollider_;                 // プレイヤーのコライダー
    std::unique_ptr<Overdrive>                          pOverdrive_;                // オーバードライブスキル
    std::unique_ptr<UpTempo>                            pUpTempo_;                  // アップテンポスキル
    std::unique_ptr<HPComponent>                        pHPComponent_;              // HPコンポーネント
    std::unique_ptr<ParryHistory>                       pParryHistory_;             // パリィヒット記録クラス
    std::unique_ptr<ParryJudgement>                     pParryJudgement_;           // パリィ判定クラス
    std::unique_ptr<ParryPresentation>                  pParryPresentation_;         // パリィエフェクト管理クラス
    
    /// ステート
    std::unique_ptr<StateMachine<PlayerStateContext>>   pStateMachine_;         // プレイヤーの状態遷移を管理するステートマシン
    std::array<std::unique_ptr<IPlayerState>, kNumStates> states_;              // プレイヤーの各状態

    /// デバッグ表示用
    GameParameterView(Tako::Transform,  transform_, {});                // キャラクターのトランスフォーム
    GameParameterView(Tako::Transform,  colliderTransform_, {});        // キャラクターのトランスフォーム
    GameParameterView(Tako::Vector3,    directionAtackSpawning, {});    // 攻撃生成の方向（デバッグ表示用）
    GameParameterView(Tako::Vector3,    jointPosition_, {});            // ジョイントの位置（デバッグ表示用）

    /// 参照
    AttackRepository&       attackRepository_;          // 攻撃リポジトリの参照
    FollowCamera&           followCamera_;              // フォローカメラの参照
    ComboBuffSystem&        comboBuffSystem_;           // コンボバフシステムの参照
    BeatClock&              beatClock_;                 // ビートクロックの参照
    Tako::EmitterManager&   particleEmitter_;           // パーティクルエミッターの参照
};
