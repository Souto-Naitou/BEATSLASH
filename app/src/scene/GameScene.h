#pragma once
#include "BaseScene.h"

#include <stage/StageSequence.h>
#include <character/player/Player.h>

#include <memory>
#include <character/enemy/Enemy.h>
#include <physics/ColliderRepository.h>

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
	std::unique_ptr<Enemy> pEnemy_; // !< 敵
    std::unique_ptr<StageSequence> pStage_; // !< ステージクラスのインスタンス
    ColliderRepository colliderRepository_; // !< コライダーリポジトリのインスタンス
};
