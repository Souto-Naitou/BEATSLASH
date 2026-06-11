#pragma once
#include <character/ICharacter.h>

class BeatClock;
namespace Tako { class EmitterManager; }
#include <Object3d.h>
#include <BehaviorTree.h>
#include <character/boss/collider/BossCollider.h>
#include <character/boss/BeatEdgeDetector.h>
#include <component/HPComponent.h>
#include <component/PhysicsMovement.h>
#include <functional>
#include <vector>

/// <summary>
/// ビヘイビアツリーで行動するボス
/// 拍境界の検知結果をブラックボードへ共有し、ノード側でリズム連携できるようにする
/// </summary>
class Boss : public ICharacter
{
public:
    Boss(const ICharacter* target, const BeatClock* beatClock = nullptr, Tako::EmitterManager* emitterManager = nullptr);
    ~Boss() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;

    /// <summary>
    /// モデルの取得
    /// </summary>
    Tako::Object3d* GetModel() { return pModel_.get(); }

    /// <summary>
    /// 各種トランスフォームの取得
    /// </summary>
    Tako::Transform& GetTransform() { return transform_; }
    const Tako::Vector3& GetPosition() const override { return transform_.translate; }
    const Tako::Vector3& GetRotation() const override { return transform_.rotate; }
    const Tako::Vector3& GetScale() const override { return transform_.scale; }

    /// <summary>
    /// 各種トランスフォームの設定
    /// </summary>
    void SetTransform(const Tako::Transform& transform) { transform_ = transform; }
    void SetPosition(const Tako::Vector3& position) { transform_.translate = position; }
    void SetRotation(const Tako::Vector3& rotation) { transform_.rotate = rotation; }
    void SetScale(const Tako::Vector3& scale) { transform_.scale = scale; }

    bool IsAlive() const { return pHp_ && pHp_->IsAlive(); }

    /**
     * @brief HPコンポーネントの取得
     */
    HPComponent* GetHPComponent() { return pHp_.get(); }

    /**
     * @brief ビートクロックの取得
     */
    const BeatClock* GetBeatClock() const { return pBeatClock_; }

    /**
     * @brief ビヘイビアツリーの取得
     */
    Tako::BehaviorTree* GetBehaviorTree() { return pBehaviorTree_.get(); }

    /**
     * @brief ノードエディタ等が構築したランタイムツリーへ差し替える
     * @param root 新しいルートノード
     */
    void SetBehaviorTreeRoot(Tako::BTNodePtr root);

    /**
     * @brief このフレームで追加描画するモデルを積む（BTノードの攻撃演出用。毎フレームUpdate冒頭でクリアされる）
     */
    void QueueAttachedModelDraw(Tako::Object3d* model) { frameAttachedModels_.push_back(model); }

    /**
     * @brief デバッグ用のImGui描画を行う。
     */
    void DrawImGui();

    /**
     * @brief ノードエディタ表示ボタン押下時のコールバックを設定する（デバッグ用）
     */
    void SetNodeEditorToggleCallback(std::function<void()> callback) { nodeEditorToggleCallback_ = callback; }

private:
    // ブラックボードへ共有ポインタ群を登録する
    void SetupBlackboard();

    // 拍同期の拡縮アニメーション更新
    void UpdateBeatAnimation();

private: // 定数定義
    // 初期化時の座標
    static constexpr Tako::Vector3 kInitialTranslate = { 0.0f, 10.0f, 0.0f };
    // 初期化時のスケール
    static constexpr Tako::Vector3 kInitialScale = { 5.0f, 5.0f, 5.0f };
    // 初期化時のマテリアルカラー
    static constexpr Tako::Vector4 kInitialMaterialColor = { 256, 0, 256, 256 };
    // コライダーのスケールの倍率
    static constexpr float kColliderScaleMultiplier = 1.3f;
    // 初期HP
    static constexpr int32_t kInitialHP = 300;
    // 質量（重力計算用）
    static constexpr float kMass = 60.0f;
    // ビヘイビアツリー定義のJSONパス
    static constexpr const char* kBehaviorTreeJsonPath = "resources/Json/BT/BossTree.json";

private:
    // モデル
    std::unique_ptr<Tako::Object3d> pModel_;
    // トランスフォーム
    Tako::Transform transform_;
    // コライダー
    std::unique_ptr<BossCollider> pCollider_;
    // 速度・加速度に基づく移動コンポーネント
    std::unique_ptr<PhysicsMovement> pMovement_;
    // BTノードがこのフレームで追加描画を要求したモデル（所有しない）
    std::vector<Tako::Object3d*> frameAttachedModels_;
    // ビヘイビアツリー
    std::unique_ptr<Tako::BehaviorTree> pBehaviorTree_;
    // 拍境界の検知（リズム連携基盤）
    BeatEdgeDetector beatEdge_;
    // ターゲット（所有しない）
    const ICharacter* pTarget_ = nullptr;
    // ビートクロックのポインタ（所有しない）
    const BeatClock* pBeatClock_ = nullptr;
    // エミッターマネージャー（所有しない）
    Tako::EmitterManager* pEmitterManager_ = nullptr;
    // HP
    std::unique_ptr<HPComponent> pHp_;

    // 拡縮アニメーション用パラメータ
    float baseScale_ = 2.0f;
    float scaleAmplitude_ = 0.1f;

    // ノードエディタ表示ボタン押下時のコールバック（デバッグ用）
    std::function<void()> nodeEditorToggleCallback_;
};
