#include "EnemyIdleState.h"

void EnemyIdleState::Enter(Enemy* enemy)
{
	timer_ = 0.0f; // タイマーをリセット
}

void EnemyIdleState::Update(Enemy* enemy)
{
	// モデルを少し大きくさせたり小さくさせてリズムに乗っているような動きをさせる

}
