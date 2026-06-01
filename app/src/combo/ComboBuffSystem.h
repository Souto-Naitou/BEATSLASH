#pragma once

#include <cstdint>

class ComboSystem;
class InputTimingJudge;
class BeatClock;

class ComboBuffSystem
{
public:

    ComboBuffSystem(ComboSystem* comboSystem, InputTimingJudge* inputTimingJudge, BeatClock* beatClock);

    float GetDamageMultiplier() const; // ダメージ倍率の取得

    uint32_t GetCurrentCombo() const;// 現在のコンボ数の取得

    void OnAttackHit(); // 攻撃ヒット時の処理

    void OnDamaged(); // ダメージを受けたときの処理
private:

    ComboSystem*        pComboSystem_       = nullptr; // コンボシステムの参照
    InputTimingJudge*   pInputTimingJudge_  = nullptr; // 入力判定クラスの参照
    BeatClock*          pBeatClock_         = nullptr; // ビートクロックの参照

};