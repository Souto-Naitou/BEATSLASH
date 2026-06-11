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

    /**
     * @brief デバッグ用のImGui描画を行う。
     * @param stageIndex ステージのインデックス
     */
    void DrawImGui(uint32_t stageIndex);

    /**
     * @brief フィールド上の敵の数を取得する。
     * @return 敵の数
     */
    size_t GetCount() const
    {
        return enemies_.size();
    }

	// 敵のリストへのアクセス
	std::vector <std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }

private:

    std::vector<std::unique_ptr<Enemy>> enemies_;

};
