#pragma once
#include <Model.h>
#include <Vector3.h>

struct PlayerAttackRequest
{
    // 生成する攻撃の中心座標
    const Tako::Vector3& position;
    // アニメーション再生用のモデル
    Tako::Model& model;
};