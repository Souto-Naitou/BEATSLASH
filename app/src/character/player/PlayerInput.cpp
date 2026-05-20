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
    PlayerInput::Data preData = data_;
    data_ = {};

    /// [ ゲームパッドモードの切り替え ]


    /// [ プレイヤーの入力用に変換 ]
    // 移動
    auto dirX = pInput_->PushKey(DIK_D) - pInput_->PushKey(DIK_A);
    auto dirZ = pInput_->PushKey(DIK_W) - pInput_->PushKey(DIK_S);
    data_.move = Tako::Vector3(static_cast<float>(dirX), 0.0f, static_cast<float>(dirZ));
    data_.move = data_.move.Normalize();
    data_.isJumpTriggered = pInput_->TriggerKey(DIK_SPACE);
    if (pInput_->PushKey(DIK_SPACE))
    {
        data_.isJumpPressed = true;
        data_.jumpHoldTime = preData.isJumpPressed ? preData.jumpHoldTime + Tako::FrameTimer::GetInstance()->GetDeltaTime() : 0.0f;
    }
    else
    {
        data_.isJumpPressed = false;
        data_.jumpHoldTime = 0.0f;
    }

}

void PlayerInput::ImGui()
{
#ifdef _DEBUG

    ImGui::InputFloat3("Move", &data_.move.x, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGuiTemplate::TextBoolean("Jump Triggered", data_.isJumpTriggered);
    ImGuiTemplate::TextBoolean("Jump Pressed", data_.isJumpPressed);
    ImGui::InputFloat("Jump Hold Time", &data_.jumpHoldTime, 0.01f, 0.1f, "%.2f", ImGuiInputTextFlags_ReadOnly);

#endif // _DEBUG
}
