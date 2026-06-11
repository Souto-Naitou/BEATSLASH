#pragma once
#include <cstdint>

class HPComponent
{
public:

    /// <summary>
    /// HPコンポーネントの初期化
    /// </summary>
    /// <param name="maxHP">最大HP</param>
    void Initialize(int32_t maxHP);

    /// <summary>
    /// ダメージを受ける
    /// </summary>
    /// <param name="damageAmount">受けるダメージ量</param>
    void Damage(int32_t damageAmount);

    /// 現在のHPを取得
    int32_t GetCurrentHP() const { return currentHP_; }

    /// 最大HP割合を取得
    float GetHPRatio() const;

    /// <summary>
    /// 生きていますか？
    /// </summary>
    /// <returns>生きている場合は true、死んでいる場合は false</returns>
    bool IsAlive() const { return currentHP_ > 0; }

private:
    int32_t maxHP_ = 100; // 最大HP
    int32_t currentHP_ = 100; // 現在のHP
};