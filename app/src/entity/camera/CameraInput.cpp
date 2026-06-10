#include "CameraInput.h"
#include <WinApp.h>
#include <winuser.h>

#include <utility/wndutl.h>

void CameraInput::Update()
{
    POINT pointCenter = utl::window::GetCenterOfWindow();
    Tako::Vector2 center = { static_cast<float>(pointCenter.x), static_cast<float>(pointCenter.y) };

    /// パッド入力 
    Tako::Vector2 padDelta = { 0.0f, 0.0f };
    if (pInput_->IsConnect())
    {
        padDelta = pInput_->GetRightStick();
        if (padDelta.Length() < 0.15f) // スティックのデッドゾーン
        {
            padDelta = { 0.0f, 0.0f };
        }
        padDelta.y*= -1.0f; // スティックの上下は逆になるので反転
        padDelta *= 5.0f; // スティックの値を適当な倍率で調整
    }


    auto cursorPos = pInput_->GetMousePos();
    auto cursorDelta = cursorPos - center;
    Tako::Vector2 totalDelta = cursorDelta + padDelta;
    command_.delta.yaw = totalDelta.x * sensitivityX;
    command_.delta.pitch = totalDelta.y * sensitivityY;
#ifdef _DEBUG
    command_.isCameraActivationTriggered = pInput_->TriggerMouse(1) // 右クリックでカメラ操作有効
                                        || pInput_->TriggerButton(Tako::GamepadButton::R_Thumbstick); // 右スティック押込でカメラ操作有効
#endif
}
