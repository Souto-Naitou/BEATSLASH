#pragma once
#include <Vector3.h>

class ICharacter
{
public:
    ICharacter() = default;
    virtual ~ICharacter() = default;

    /// <summary>
    /// キャラクターの初期化
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>
    /// キャラクターの更新
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// キャラクターの描画
    /// </summary>
    virtual void Draw() = 0;

    /**
     * @brief 座標の取得
     */
    virtual const Tako::Vector3& GetPosition() = 0;

	/**
	 * @brief スケールの取得
	 */
	virtual const Tako::Vector3& GetScale() = 0;

	/**
	 * @brief 回転の取得
	 */
	virtual const Tako::Vector3& GetRotation() = 0;
};
