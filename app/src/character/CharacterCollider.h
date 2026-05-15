#pragma once

#include <AABBCollider.h>
#include <utility/CollisionUtility.h>
#include "CharacterColliderID.h"
#include <functional>

using CollisionCallback = std::function<void(const Tako::Collider&)>;

class CharacterCollider : public Tako::OBBCollider
{
public:
	CharacterCollider() = default;
	~CharacterCollider() = default;

	void OnCollisionEnter(Tako::Collider* other) override
	{
		// コールバックの呼び出し
		if (onCollisionEnterCallback_)
		{
			onCollisionEnterCallback_(*other);
		}
	}

	void OnCollisionStay(Tako::Collider* other) override
	{
		// コールバックの呼び出し
		if (onCollisionStayCallback_)
		{
			onCollisionStayCallback_(*other);
		}
	}

	void OnCollisionExit(Tako::Collider* other) override
	{
		// コールバックの呼び出し
		if (onCollisionExitCallback_)
		{
			onCollisionExitCallback_(*other);
		}
	}

	// コールバック関数の設定
	void SetOnCollisionEnterCallback(CollisionCallback callback) { onCollisionEnterCallback_ = std::move(callback); }
	void SetOnCollisionStayCallback(CollisionCallback callback) { onCollisionStayCallback_ = std::move(callback); }
	void SetOnCollisionExitCallback(CollisionCallback callback) { onCollisionExitCallback_ = std::move(callback); }

private:
	// 衝突時のコールバック関数
	CollisionCallback onCollisionEnterCallback_;
	CollisionCallback onCollisionStayCallback_;
	CollisionCallback onCollisionExitCallback_;

};