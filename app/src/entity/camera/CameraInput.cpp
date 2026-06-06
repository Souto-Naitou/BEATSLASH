#include "CameraInput.h"
#include <WinApp.h>
#include <winuser.h>

#include <utility/wndutl.h>

void CameraInput::Update()
{
    POINT pointCenter = utl::window::GetCenterOfWindow();
    Tako::Vector2 center = { static_cast<float>(pointCenter.x), static_cast<float>(pointCenter.y) };

    auto cursorPos = pInput_->GetMousePos();
    auto cursorDelta = cursorPos - center;
    command_.delta.yaw = cursorDelta.x * sensitivityX;
    command_.delta.pitch = cursorDelta.y * sensitivityY;

    command_.isCameraActivationTriggered = pInput_->TriggerMouse(1); // 右クリックでカメラ操作有効
}
