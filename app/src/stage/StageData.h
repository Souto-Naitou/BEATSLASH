
#pragma once

#include <Transform.h>
#include <vector>

struct StageData
{
    std::vector<Tako::Transform> floorTransform = {};      // 床のトランスフォーム
    Tako::Transform transitionTransform = {}; // 遷移コライダーのトランスフォーム（translate=位置, scale=サイズ）
    Tako::Transform doorTransform = {};       // ドアのトランスフォーム
    Tako::Transform playerStartTransform = {}; // プレイヤーの初期位置のトランスフォーム
    bool surroundWalls = false;          // 床の周囲を見えない壁コライダーで囲むか
    bool spawnBoss = false;              // このステージでボスを生成するか
};