#include "EnemiesOnField.h"

#include <character/enemy/Enemy.h>

void EnemiesOnField::Add(std::unique_ptr<Enemy> enemy)
{
    if (enemy)
    {
        enemies_.push_back(std::move(enemy));
    }
}

void EnemiesOnField::Update()
{
    for (auto it = enemies_.begin(); it != enemies_.end(); )
    {
        if ((*it) && (*it)->IsAlive())
        {
            (*it)->Update();
            ++it;

        }
        else
        {
            // 死亡している敵をリストから削除
            it = enemies_.erase(it);
        }
    }
}

void EnemiesOnField::Draw()
{
    for (auto& enemy : enemies_)
    {
        if (enemy)
        {
            enemy->Draw();
        }
    }
}

bool EnemiesOnField::IsEmpty() const
{
    return enemies_.empty();
}
