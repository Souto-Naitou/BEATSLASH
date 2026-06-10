#pragma once

#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <character/ICharacter.h>
#include <optional>

class EnemyIdleState : public EnemyState
{
public:	
	EnemyIdleState(const ICharacter* target) : pTarget_(target) {}
	std::optional<EnemyStateType> CheckTransition(Enemy* enemy) override;
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;

private:
	// 追跡を開始する距離
	static constexpr float kChaseStartDistance = 20.0f;
	// ターゲットのポインタ
	const ICharacter* pTarget_ = nullptr;

};

