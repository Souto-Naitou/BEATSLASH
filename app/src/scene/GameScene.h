#pragma once
#include "BaseScene.h"

#include <stage/StageSequence.h>
#include <character/player/Player.h>

#include <memory>
#include <physics/ColliderRepository.h>
#include <entity/attack/AttackRepository.h>
#include <character/player/FollowCamera.h>
#include <entity/enemy/EnemiesOnField.h>

#include <combo/ComboSystem.h>
#include <judge/InputTimingJudge.h>
#include <manager/BeatManager.h>
#include <combo/ComboBuffSystem.h>
#include <ui/GameHUD.h>

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

private: // メンバ変数
    std::unique_ptr<Player> pPlayer_; // !< プレイヤー
	std::unique_ptr<EnemiesOnField> pEnemies_; // !< 敵
    std::unique_ptr<StageSequence> pStage_; // !< ステージクラスのインスタンス
    std::unique_ptr<AttackRepository> pAttackRepository_; // !< コライダーリポジトリのインスタンス
    std::unique_ptr<FollowCamera> pFollowCamera_; // !< カメラクラスのインスタンス
    ColliderRepository colliderRepository_; // !< 攻撃リポジトリのインスタンス

    std::unique_ptr<ComboSystem> pComboSystem_; // !< コンボシステム
    std::unique_ptr<InputTimingJudge> pInputTimingJudge_; // !< 入力判定クラス
    std::unique_ptr<BeatClock> pBeatClock_; // !< ビートクロック
    std::unique_ptr<ComboBuffSystem> pComboBuffSystem_; // !< コンボバフシステム

    std::unique_ptr<GameHUD> pGameHUD_; // !< ゲームHUD

};
