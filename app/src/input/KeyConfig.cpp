#include "KeyConfig.h"
#include <Input.h>

const std::unordered_map<PlayerAction, KeyboardBinding> KeyConfig::kKeyboardBindings_ =
{
    { PlayerAction::MoveForward, { DIK_W,     -1,"kb/W.png"     ,"",false} },
    { PlayerAction::MoveBack,    { DIK_S,     -1,"kb/S.png"     ,"",false} },
    { PlayerAction::MoveLeft,    { DIK_A,     -1,"kb/A.png"     ,"",false} },
    { PlayerAction::MoveRight,   { DIK_D,     -1,"kb/D.png"     ,"",false} },
    { PlayerAction::Jump,        { DIK_SPACE, -1,"key\\key_Space.png" ,"key\\key_back.png",false} },
    { PlayerAction::Attack,      { DIK_E,      0,"key\\mouse_button.png","",false} },
    { PlayerAction::Overdrive,   { DIK_F,     -1,"key\\key_F.png"     ,"key\\key_back.png",false} },
    { PlayerAction::UpTempo,     { DIK_G,     -1,"key\\key_G.png"     ,"key\\key_back.png",false} },
    { PlayerAction::Parry,       { DIK_LSHIFT, 1,"key\\mouse_button.png","",true} },
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
    { PlayerAction::Parry,       { Tako::GamepadButton::L_Shoulder,     "key\\pad_LB.png"     ,"key\\pad_bumper_back_.png"      ,AnalogTrigger::None,true} },
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
    {
        return pInput->TriggerKey(kb.dikKey)
            || (kb.mouseButton != -1 && pInput->TriggerMouse(kb.mouseButton));
    }

    if (pd.button != 0)
        return pInput->TriggerButton(pd.button);

    switch (pd.analogTrigger)
    {
        case AnalogTrigger::Right: return pInput->GetRightTrigger() > 0.5f;
        case AnalogTrigger::Left:  return pInput->GetLeftTrigger() > 0.5f;
        default:                   return false;
    }
}