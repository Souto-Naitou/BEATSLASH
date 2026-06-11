#pragma once
#include "BaseScene.h"

#include <stage/StageSequence.h>
#include <character/player/Player.h>
#include <character/boss/Boss.h>

#ifdef _DEBUG
#include <BehaviorTreeEditor.h>
#endif

#include <memory>
#include <physics/ColliderRepository.h>
#include <entity/attack/AttackRepository.h>
#include <entity/camera/CameraDirector.h>
#include <entity/enemy/EnemyManager.h>

#include <combo/ComboSystem.h>
#include <judge/InputTimingJudge.h>
#include <manager/BeatClock.h>
#include <combo/ComboBuffSystem.h>
#include <ui/GameHUD.h>
#include <factory/PlayerAttackFactory.h>
#include <EmitterManager.h>
#include <wrapper/InputAwareSprite.h>
#include <math/Color.h>

/// <summary>
/// サンプルシーンクラス
/// TakoEngineのデフォルトシーンの構成を参考にしてください。
/// </summary>
class GameScene : public Tako::BaseScene
{
public: // メンバ関数

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    void DrawWithoutEffect() override;

    /// <summary>
    /// ImGuiの描画
    /// </summary>
    void DrawImGui() override;

private: // メンバ関数
    void LoadParticleEmitterPresets();
    void LoadImageAll();
    void SpawnBoss();
    void DrawObjects();
    void ApplyPostEffects();
    void InitializeSprites();
    void UpdateBackgroundColor();

private: // メンバ変数
    std::unique_ptr<Tako::EmitterManager>   pEmitterManager_;           // !< エミッターマネージャー
    std::unique_ptr<Player>                 pPlayer_;                   // !< プレイヤー
    std::unique_ptr<EnemyManager> pEnemyManager_;                       // !< 敵のマネージャー
    std::unique_ptr<Boss>                   pBoss_;                     // !< ボス
    std::unique_ptr<StageSequence>          pStage_;                    // !< ステージクラスのインスタンス
    std::unique_ptr<AttackRepository>       pAttackRepository_;         // !< コライダーリポジトリのインスタンス
    std::unique_ptr<FollowCamera>           pFollowCamera_;             // !< カメラクラスのインスタンス
    std::unique_ptr<ComboSystem>            pComboSystem_;              // !< コンボシステム
    std::unique_ptr<InputTimingJudge>       pInputTimingJudge_;         // !< 入力判定クラス
    std::unique_ptr<BeatClock>              pBeatClock_;                // !< ビートクロック
    std::unique_ptr<ComboBuffSystem>        pComboBuffSystem_;          // !< コンボバフシステム
    std::unique_ptr<PlayerAttackFactory>    pPlayerAttackFactory_;      // !< プレイヤー攻撃ファクトリー
    std::unique_ptr<GameHUD>                pGameHUD_;                  // !< ゲームHUD
    std::unique_ptr<CameraDirector>         pCameraDirector_;           // !< カメラディレクターのインスタンス
    std::unique_ptr<Tako::Sprite>           pSpriteCursorSwitchGuide_;  // !< カーソルスイッチのガイドスプライト
    std::unique_ptr<Tako::Sprite>           pSpriteBackground_;         // !< 背景スプライト
    InputAwareSprite                        inputAwareSprite_;          // !< 入力に反応してエフェクトを出すスプライト
    HSV                                     backgroundColor_;           // !< 背景色（HSV）


    ColliderRepository colliderRepository_; // !< 攻撃リポジトリのインスタンス

    float elapsedTime_    = 0.0f;  // ゲーム開始からの経過時間（秒）
    float bossDeathTimer_ = 0.0f;
    bool isBossDeathStarted_ = false;
    bool isClearHandled_ = false;  // クリア時のランキング記録・シーン遷移を一度だけ行うためのフラグ
    Tako::Vector3 bossDeathStartScale_{ 5.0f, 5.0f, 5.0f };
    static constexpr float kDeltaHue_ = 0.05f; // 背景色の色相変化量
    static constexpr float kMinValue_ = 32.0f; // 背景色の明度 - 最小値
    static constexpr float kMaxValue_ = 35.0f; // 背景色の明度 - 最大値

#ifdef _DEBUG
    std::unique_ptr<Tako::BehaviorTreeEditor> pBtEditor_;           // !< ビヘイビアツリーエディタ
#endif
};
