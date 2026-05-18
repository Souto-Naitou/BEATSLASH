
#pragma once

#include <Transform.h>
#include <vector>

struct StageData
{
    std::vector<Tako::Transform> floorTransform;      // 床のトランスフォーム
    Tako::Transform transitionTransform; // 遷移コライダーのトランスフォーム（translate=位置, scale=サイズ）
    Tako::Transform doorTransform;       // ドアのトランスフォーム
};