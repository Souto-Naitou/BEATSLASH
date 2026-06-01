#include "EnemySpawner.h"

std::unique_ptr<Enemy> EnemySpawner::Spawn(const Tako::Vector3& position)
{
	auto enemy = std::make_unique<Enemy>(pTarget_);
	enemy->Initialize();
	enemy->SetPosition(position);
	return enemy;
}

std::unique_ptr<Enemy> EnemySpawner::Spawn(const Tako::Transform& transform)
{
	auto enemy = std::make_unique<Enemy>(pTarget_);
	enemy->Initialize();
	enemy->SetTransform(transform);
	return enemy;
}
