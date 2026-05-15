
#pragma once

#include <Vector3.h>
#include <Transform.h>

struct StageData
{
    Tako::Transform transitionTransform; // 遷移コライダーのトランスフォーム（translate=位置, scale=サイズ）
    Tako::Transform doorTransform;       // ドアのトランスフォーム
};