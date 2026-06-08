#pragma once
#include <character/ICharacter.h>

class BeatClock;
#include <character/enemy/Enemy.h>

class EnemySpawner
{
public:
	EnemySpawner(const ICharacter* target = nullptr, const BeatClock* beatClock = nullptr)
		: pTarget_(target), pBeatClock_(beatClock) {}
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
};

