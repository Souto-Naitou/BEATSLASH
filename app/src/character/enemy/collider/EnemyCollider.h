#pragma once
#include <OBBCollider.h>
#include <functional>

using EnemyPushBackCallback = std::function<void(const Tako::Vector3& pushback)>;

class HPComponent;

class EnemyCollider : public Tako::OBBCollider
{
public:
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	void SetPushBackCallback(EnemyPushBackCallback callback) { pushBackCallback_ = callback; }
    void SetHPComponent(HPComponent* hp) { pHp_ = hp; }
private:
	EnemyPushBackCallback pushBackCallback_;

    HPComponent* pHp_ = nullptr;
};

