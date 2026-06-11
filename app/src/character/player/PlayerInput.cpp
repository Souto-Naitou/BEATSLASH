#include "PlayerInput.h"
#include <FrameTimer.h>
#include <input/KeyConfig.h>

#ifdef _DEBUG
#include <DebugUIManager.h>
#include <debug/ImGuiTextTemplate.h>
#include <imgui.h>
#endif // _DEBUG


void PlayerInput::Initialize()
{
    pInput_ = Tako::Input::GetInstance();
}

void PlayerInput::Update()
{
    #ifdef _DEBUG
    if (!Tako::DebugUIManager::GetInstance()->IsCursorOverGameView()) return;
    #endif // _DEBUG

    // 前回の入力データ
    PlayerInput::PlayerCommand preData = data_;
    data_ = {};

    /// [ ゲームパッドモードの動的切り替え ]
    Tako::Vector3 padDir = { 0.0f,0.0f,0.0f };
    if (pInput_->IsConnect())
    {
        Tako::Vector2 leftStick = pInput_->GetLeftStick();
        if (leftStick.Length() > 0.15f)
            padDir = Tako::Vector3(leftStick.x, 0.0f, leftStick.y);

        // パッド入力があればパッドモードへ
        bool anyPadInput = !pInput_->LStickInDeadZone()
            || pInput_->GetRightTrigger() > 0.1f
            || pInput_->GetLeftTrigger() > 0.1f;
        if (!anyPadInput)
        {
            for (auto btn : Tako::GamepadButton::ALL)
            {
                if (pInput_->TriggerButton(btn)) { anyPadInput = true; break; }
            }
        }
        if (anyPadInput) isGamepadMode_ = true;
    }

    // キーボード入力があればキーボードモードへ
    constexpr PlayerAction kAllActions[] = {
        PlayerAction::MoveForward, PlayerAction::MoveBack,
        PlayerAction::MoveLeft,    PlayerAction::MoveRight,
        PlayerAction::Jump,        PlayerAction::Attack,
        PlayerAction::Overdrive,   PlayerAction::UpTempo,
    };
    for (auto action : kAllActions)
    {
        if (pInput_->TriggerKey(KeyConfig::GetKeyboard(action).dikKey))
        {
            isGamepadMode_ = false;
            break;
        }
    }

    /// [ プレイヤーの入力用に変換 ]
    // 移動
    const auto& kbFwd  = KeyConfig::GetKeyboard(PlayerAction::MoveForward);
    const auto& kbBack = KeyConfig::GetKeyboard(PlayerAction::MoveBack);
    const auto& kbLeft = KeyConfig::GetKeyboard(PlayerAction::MoveLeft);
    const auto& kbRight= KeyConfig::GetKeyboard(PlayerAction::MoveRight);
    auto dirX = pInput_->PushKey(kbRight.dikKey) - pInput_->PushKey(kbLeft.dikKey);
    auto dirZ = pInput_->PushKey(kbFwd.dikKey) - pInput_->PushKey(kbBack.dikKey);
    data_.move = Tako::Vector3(static_cast<float>(dirX), 0.0f, static_cast<float>(dirZ)) + padDir;
    data_.move = data_.move.Normalize();

    // ジャンプ
    const auto& kbJump  = KeyConfig::GetKeyboard(PlayerAction::Jump);
    const auto& padJump = KeyConfig::GetPad(PlayerAction::Jump);
    data_.isJumpTriggered = pInput_->TriggerKey(kbJump.dikKey) || pInput_->TriggerButton(padJump.button);
    if (pInput_->PushKey(kbJump.dikKey) || pInput_->PushButton(padJump.button))
    {
        data_.isJumpPressed = true;
        data_.jumpHoldTime = preData.isJumpPressed ? preData.jumpHoldTime + Tako::FrameTimer::GetInstance()->GetDeltaTime() : 0.0f;
    }
    else
    {
        data_.isJumpPressed = false;
        data_.jumpHoldTime = 0.0f;
    }

    // 攻撃
    const auto& kbAttack  = KeyConfig::GetKeyboard(PlayerAction::Attack);
    const auto& padAttack = KeyConfig::GetPad(PlayerAction::Attack);
    data_.isAttackTriggered = pInput_->TriggerKey(kbAttack.dikKey) || (kbAttack.mouseButton != -1 && pInput_->TriggerMouse(kbAttack.mouseButton));
    data_.isAttackTriggered |= pInput_->TriggerButton(padAttack.button);
    // オーバードライブ（アナログトリガーはパッド側で個別処理）
    const auto& kbOd = KeyConfig::GetKeyboard(PlayerAction::Overdrive);
    data_.isOverdriveTriggered = pInput_->TriggerKey(kbOd.dikKey) || pInput_->GetRightTrigger() > 0.5f;

    // アップテンポ
    const auto& kbUt = KeyConfig::GetKeyboard(PlayerAction::UpTempo);
    data_.isUpTempoTriggered = pInput_->TriggerKey(kbUt.dikKey) || pInput_->GetRightTrigger() > 0.5f;

    const auto& kbParry = KeyConfig::GetKeyboard(PlayerAction::Parry);
    const auto& padParry = KeyConfig::GetPad(PlayerAction::Parry);
    data_.isParryTriggered = pInput_->TriggerKey(kbParry.dikKey) || (kbParry.mouseButton != -1 && pInput_->TriggerMouse(kbParry.mouseButton));
    data_.isParryTriggered |= pInput_->TriggerButton(padParry.button);
}

void PlayerInput::ImGui()
{
#ifdef _DEBUG

    ImGui::InputFloat3("Move", &data_.move.x, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGuiTemplate::TextBoolean("Jump Triggered", data_.isJumpTriggered);
    ImGuiTemplate::TextBoolean("Jump Pressed", data_.isJumpPressed);
    ImGui::InputFloat("Jump Hold Time", &data_.jumpHoldTime, 0.01f, 0.1f, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGuiTemplate::TextBoolean("Overdrive Triggered", data_.isOverdriveTriggered);
    ImGuiTemplate::TextBoolean("UpTempo Triggered", data_.isUpTempoTriggered);

#endif // _DEBUG
}
