#pragma once
#include <Object3d.h>
#include <characters/base/CharacterCollider.h>
#include <characters/base/CharacterColliderID.h>

class Character
{
public:
	Character() = default;
	virtual ~Character() = default;
	/// <summary>
	/// キャラクターの初期化
	/// </summary>
	virtual void Initialize(CharacterColliderID colliderID);
	/// <summary>
	/// キャラクターの更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// キャラクターの描画
	/// </summary>
	virtual void Draw();


	/// <summary>
	/// トランスフォームの取得
	/// </summary>
	/// <returns></returns>
	const Tako::Transform& GetTransform() const { return transform_; }
	const Tako::Vector3& GetPosition() const { return transform_.translate; }
	const Tako::Vector3& GetRotation() const { return transform_.rotate; }
	const Tako::Vector3& GetScale() const { return transform_.scale; }

	/// <summary>
	/// トランスフォームの設定
	/// </summary>
	/// <param name="transform"></param>
	void SetTransform(const Tako::Transform& transform) { transform_ = transform; }
	void SetPosition(const Tako::Vector3& position) { transform_.translate = position; }
	void SetRotation(const Tako::Vector3& rotation) { transform_.rotate = rotation; }
	void SetScale(const Tako::Vector3& scale) { transform_.scale = scale; }

	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns></returns>
	CharacterCollider* GetCollider() const { return collider_.get(); }

private:
	std::unique_ptr<Tako::Object3d> model_;			 ///< キャラクターの3Dモデル
	Tako::Transform transform_;						 ///< キャラクターのトランスフォーム
	std::unique_ptr<CharacterCollider> collider_;	 ///< キャラクターのコライダー
};
