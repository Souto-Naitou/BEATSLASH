#include "KeyConfig.h"
#include <Input.h>

const std::unordered_map<PlayerAction, KeyboardBinding> KeyConfig::kKeyboardBindings_ =
{
    { PlayerAction::MoveForward, { DIK_W,     "kb/W.png"     ,""} },
    { PlayerAction::MoveBack,    { DIK_S,     "kb/S.png"     ,""} },
    { PlayerAction::MoveLeft,    { DIK_A,     "kb/A.png"     ,""} },
    { PlayerAction::MoveRight,   { DIK_D,     "kb/D.png"     ,""} },
    { PlayerAction::Jump,        { DIK_SPACE, "key\\key_Space.png" ,"key\\key_back.png"} },
    { PlayerAction::Attack,      { DIK_E,     "key\\key_E.png"     ,"key\\key_back.png"} },
    { PlayerAction::Overdrive,   { DIK_F,     "key\\key_F.png"     ,"key\\key_back.png"} },
    { PlayerAction::UpTempo,     { DIK_G,     "key\\key_G.png"     ,"key\\key_back.png"} },
};

const std::unordered_map<PlayerAction, PadBinding> KeyConfig::kPadBindings_ =
{
    { PlayerAction::MoveRight,   { 0,                                   "key/LStick.png","" ,AnalogTrigger::None,false} },
    { PlayerAction::MoveForward, { 0,                                   "key/LStick.png","" ,AnalogTrigger::None,false} },
    { PlayerAction::MoveBack,    { 0,                                   "key/LStick.png","" ,AnalogTrigger::None,false} },
    { PlayerAction::MoveLeft,    { 0,                                   "key/LStick.png","" ,AnalogTrigger::None,false} },
    { PlayerAction::Jump,        { Tako::GamepadButton::A,              "key\\pad_A.png"      ,"key\\pad_button_back.png"       ,AnalogTrigger::None,false} },
    { PlayerAction::Attack,      { Tako::GamepadButton::R_Shoulder,     "key\\pad_RB.png"     ,"key\\pad_bumper_back_.png"      ,AnalogTrigger::None,false} },
    { PlayerAction::Overdrive,   { 0,                                   "key\\pad_RT.png"     ,"key\\pad_triger_back.png"       ,AnalogTrigger::Right,false} },
    { PlayerAction::UpTempo,     { 0,                                   "key\\pad_LT.png"     ,"key\\pad_triger_back.png"       ,AnalogTrigger::Left,true} },
};

const KeyboardBinding& KeyConfig::GetKeyboard(PlayerAction action)
{
    return kKeyboardBindings_.at(action);
}

const PadBinding& KeyConfig::GetPad(PlayerAction action)
{
    return kPadBindings_.at(action);
}

bool KeyConfig::IsActionTriggered(const PadBinding& pd, const KeyboardBinding& kb, bool isGamepad)
{
    auto pInput = Tako::Input::GetInstance();

    if (!isGamepad)
        return pInput->TriggerKey(kb.dikKey);

    if (pd.button != 0)
        return pInput->TriggerButton(pd.button);

    switch (pd.analogTrigger)
    {
        case AnalogTrigger::Right: return pInput->GetRightTrigger() > 0.5f;
        case AnalogTrigger::Left:  return pInput->GetLeftTrigger() > 0.5f;
        default:                   return false;
    }
}