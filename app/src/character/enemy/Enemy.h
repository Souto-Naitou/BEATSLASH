#pragma once
#include <character/ICharacter.h>
#include <character/enemy/state/EnemyStateMachine.h>
#include <Object3d.h>
#include <character/enemy/collider/EnemyCollider.h>
#include <component/HPComponent.h>

class Enemy : public ICharacter
{
public:
	Enemy(const ICharacter* target);
	~Enemy() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <returns></returns>
	Tako::Object3d* GetModel() { return pModel_.get(); }

	/// <summary>
	/// 各種トランスフォームの取得
	/// </summary>
	/// <returns></returns>
	Tako::Transform& GetTransform() { return transform_; }
	const Tako::Vector3& GetPosition() const override { return transform_.translate; }
	const Tako::Vector3& GetRotation() const override { return transform_.rotate; }
	const Tako::Vector3& GetScale() const override { return transform_.scale; }


	/// <summary>
	///	各種トランスフォームの設定
	/// </summary>
	/// <param name="position"></param>
	void SetTransform(const Tako::Transform& transform) { transform_ = transform; }
	void SetPosition(const Tako::Vector3& position) { transform_.translate = position; }
	void SetRotation(const Tako::Vector3& rotation) { transform_.rotate = rotation; }
	void SetScale(const Tako::Vector3& scale) { transform_.scale = scale; }

    bool IsAlive() const { return pHp_ && pHp_->IsAlive(); }
private:
	// 状態の切り替え（デバッグ用）
	bool ChangeState();

	// デバッグ表示
	void DrawImGui();

private:
	// モデル
	std::unique_ptr<Tako::Object3d> pModel_;
	// トランスフォーム
	Tako::Transform transform_;
	// コライダー
	std::unique_ptr<EnemyCollider> pCollider_;
	// 敵のステートマシン
	EnemyStateMachine stateMachine_;
	// ターゲット（所有しない）
	const ICharacter* pTarget_ = nullptr;

	// 重力
	static constexpr float kGravity = -9.8f;
  
  std::unique_ptr<HPComponent> pHp_;
};

