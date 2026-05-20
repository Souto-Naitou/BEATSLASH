#pragma once
#include <Input.h>
#include <Vector3.h>

/// <summary>
/// プレイヤー入力管理クラス
/// </summary>
class PlayerInput
{
public:
    struct Data
    {
        // 移動方向ベクトル (PlayerInput::Update()で正規化済み)
        Tako::Vector3   move = {};
        // ジャンプのトリガー状態
        bool            isJumpTriggered = false;
        // ジャンプボタンが押されている状態
        bool            isJumpPressed = false;
        // ジャンプボタンを押し続けている時間
        float           jumpHoldTime = 0.0f;
    };

    PlayerInput() = default;
    ~PlayerInput() = default;

    void Initialize();
    void Update();
    void ImGui();

    bool            IsGamepadMode() const { return isGamepadMode_; }
    const Data&     GetData() const { return data_; }

private:
    bool isGamepadMode_ = false;
    Tako::Input* pInput_ = nullptr;
    Data data_ = {};
};