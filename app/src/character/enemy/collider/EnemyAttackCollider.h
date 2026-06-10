#pragma once
#include <OBBCollider.h>
#include <string>

namespace Tako
{
	class EmitterManager;
}

class EnemyAttackCollider : public Tako::OBBCollider
{
public:
	EnemyAttackCollider(Tako::EmitterManager* emitterManager);
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;
private:
	Tako::EmitterManager* pEmitterManager_ = nullptr;
};
