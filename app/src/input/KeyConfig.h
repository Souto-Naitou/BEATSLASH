#pragma once
#include "PlayerAction.h"
#include <Xinput.h>
#include <dinput.h>
#include <string>
#include <unordered_map>

struct KeyboardBinding
{
    int         dikKey;
    std::string imagePath;
    std::string backgroundImagePath; // ボタン画像の背景に表示する画像
};
enum class AnalogTrigger { None, Left, Right };

struct PadBinding
{
    WORD        button;     // Tako::GamepadButton の定数、0 = なし
    std::string imagePath;
    std::string backgroundImagePath; // ボタン画像の背景に表示する画像
    AnalogTrigger analogTrigger = AnalogTrigger::None; // アナログトリガーの指定
    bool flipU = false; // 画像の左右反転フラグ
};

class KeyConfig
{
public:
    static const KeyboardBinding& GetKeyboard(PlayerAction action);
    static const PadBinding& GetPad(PlayerAction action);
    static bool IsActionTriggered(const PadBinding& pd, const KeyboardBinding& kb, bool isGamepad);
private:
    static const std::unordered_map<PlayerAction, KeyboardBinding> kKeyboardBindings_;
    static const std::unordered_map<PlayerAction, PadBinding>      kPadBindings_;
};
