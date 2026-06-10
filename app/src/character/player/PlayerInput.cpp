#include "PlayerInput.h"
#include <FrameTimer.h>


#ifdef _DEBUG
#include <debug/ImGuiTextTemplate.h>
#include <imgui.h>
#endif // _DEBUG


void PlayerInput::Initialize()
{
    pInput_ = Tako::Input::GetInstance();
}

void PlayerInput::Update()
{
    // 前回の入力データ
    PlayerInput::PlayerCommand preData = data_;
    data_ = {};

    /// [ ゲームパッドモードの切り替え ]
    Tako::Vector3 padDir = { 0.0f,0.0f,0.0f };
    if (pInput_->IsConnect())
    {
        Tako::Vector2 leftStick = pInput_->GetLeftStick();
        if (leftStick.Length() > 0.15f) // スティックのデッドゾーン
            padDir = Tako::Vector3(leftStick.x, 0.0f, leftStick.y);
    }


    /// [ プレイヤーの入力用に変換 ]
    // 移動
    auto dirX = pInput_->PushKey(DIK_D) - pInput_->PushKey(DIK_A);
    auto dirZ = pInput_->PushKey(DIK_W) - pInput_->PushKey(DIK_S);
    data_.move = Tako::Vector3(static_cast<float>(dirX), 0.0f, static_cast<float>(dirZ)) + padDir;
    data_.move = data_.move.Normalize();
    data_.isJumpTriggered = pInput_->TriggerKey(DIK_SPACE) || pInput_->TriggerButton(Tako::GamepadButton::A);
    if (pInput_->PushKey(DIK_SPACE) || pInput_->PushButton(Tako::GamepadButton::A))
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
    //data_.isAttackTriggered = pInput_->TriggerMouse(0);
    data_.isAttackTriggered = pInput_->TriggerKey(DIK_E) || pInput_->TriggerButton(Tako::GamepadButton::R_Shoulder);
    // オーバードライブ
    data_.isOverdriveTriggered = pInput_->TriggerKey(DIK_F) || pInput_->GetRightTrigger() != 0.5f;
    // アップテンポ
    data_.isUpTempoTriggered = pInput_->TriggerKey(DIK_G) || pInput_->GetRightTrigger() > 0.5f;
}

void PlayerInput::ImGui()
{
#ifdef _DEBUG

    ImGui::InputFloat3("Move", &data_.move.x, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGuiTemplate::TextBoolean("Jump Triggered", data_.isJumpTriggered);
    ImGuiTemplate::TextBoolean("Jump Pressed", data_.isJumpPressed);
    ImGui::InputFloat("Jump Hold Time", &data_.jumpHoldTime, 0.01f, 0.1f, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGuiTemplate::TextBoolean("Overdrive Triggered", data_.isOverdriveTriggered);

#endif // _DEBUG
}
