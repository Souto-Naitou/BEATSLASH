#pragma once

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
};
