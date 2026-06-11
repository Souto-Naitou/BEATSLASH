#pragma once
#include <character/ICharacter.h>

class BeatClock;
#include <character/enemy/Enemy.h>

namespace Tako
{
	class EmitterManager;
}

class EnemySpawner
{
public:
	EnemySpawner(const ICharacter* target = nullptr, const BeatClock* beatClock = nullptr, Tako::EmitterManager* emitterManager = nullptr)
		: pTarget_(target), pBeatClock_(beatClock), pEmitterManager_(emitterManager) {}
	~EnemySpawner() = default;

	/// <summary>
	/// 指定した位置に敵を1体生成する
	/// </summary>
	/// <param name="position"> 敵を生成する位置の座標 </param>
	/// <returns></returns>
	std::unique_ptr<Enemy> Spawn(const Tako::Vector3& position);

	/// <summary>
	/// 指定したトランスフォームで敵を1体生成する
	/// </summary>
	/// <param name="transform"></param>
	/// <returns></returns>
	std::unique_ptr<Enemy> Spawn(const Tako::Transform& transform);

private:
	// ターゲットのポインタ
	const ICharacter* pTarget_ = nullptr;
	// ビートクロックのポインタ
	const BeatClock* pBeatClock_ = nullptr;
	// エミッターマネージャーのポインタ。所有しない（GameSceneが所有し、GameSceneの寿命まで生きる）
	Tako::EmitterManager* pEmitterManager_ = nullptr;
};


