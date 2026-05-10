#pragma once
#include "BaseScene.h"

#include <Object3d.h>
#include <AABBCollider.h>
#include "testCollider.h"

/// <summary>
/// サンプルシーンクラス
/// TakoEngineのデフォルトシーンの構成を参考にしてください。
/// </summary>
class TestScene : public Tako::BaseScene
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

    std::unique_ptr<Tako::Object3d> testModel1_; ///< テスト用3Dモデル1
    std::unique_ptr<Tako::Object3d> testModel2_; ///< テスト用3Dモデル2

    Tako::Transform testModel1Transform_; ///< テスト用3Dモデル1のトランスフォーム
    Tako::Transform testModel2Transform_; ///< テスト用3Dモデル2のトランスフォーム

    std::unique_ptr<TestCollider> aabbCollider1_; ///< テスト用AABBコライダー1
    std::unique_ptr<Tako::SphereCollider> aabbCollider2_; ///< テスト用AABBコライダー2


};
