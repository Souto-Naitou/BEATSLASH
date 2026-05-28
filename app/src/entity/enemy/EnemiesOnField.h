#pragma once
#include <vector>
#include <memory>
#include <character/enemy/Enemy.h>

/// <summary>
/// フィールド上の敵を管理するクラス
/// </summary>
/// ファサードパターン想定

class EnemiesOnField
{
public:

    EnemiesOnField() = default;
    ~EnemiesOnField() = default;
    

    void Add(std::unique_ptr<Enemy> enemy);

    void Update();
    void Draw();

    bool IsEmpty() const;


private:

    std::vector<std::unique_ptr<Enemy>> enemies_;

};
