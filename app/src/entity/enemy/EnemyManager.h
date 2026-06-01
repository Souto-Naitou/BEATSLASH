#pragma once
#include "EnemiesOnField.h"
#include "EnemySpawner.h"

class EnemyManager
{
public:
	// 管理するステージの最大数(ある程度余裕を持って設定、必要に応じて調整)
	static constexpr uint32_t kMaxStages = 20;

	EnemyManager(const ICharacter* target) : pTarget_(target) {}

	void Update();

private:
	// ターゲット（プレイヤー）のポインタ
	const ICharacter* pTarget_ = nullptr;

	// フィールド上の敵の管理
	std::array<EnemiesOnField, kMaxStages> enemiesOnField_;
	
	// 敵のスポナー
	EnemySpawner spawner_{ pTarget_ };
};

