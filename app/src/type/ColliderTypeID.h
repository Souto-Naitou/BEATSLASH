#pragma once

/// <summary>
/// キャラクターのコライダーID
/// </summary>
enum class ColliderTypeID
{
	Player  = 100,				// プレイヤー
	Enemy,				// 敵
	Obstacle,			// 障害物
    Terrain,			// 地形
};