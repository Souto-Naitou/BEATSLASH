#pragma once

/// <summary>
/// キャラクターのコライダーID
/// </summary>
enum class ColliderTypeID
{
    Player  = 100,				// プレイヤー
    PlayerAttack,		// プレイヤーの攻撃
    Enemy,				// 敵
    Obstacle,			// 障害物
    Terrain,			// 地形
};