#include "CameraInput.h"



void CameraInput::Update()
{
    auto cursorPos = pInput_->GetMousePos();

    auto cursorDelta = cursorPos - cursorPosPrev_;

    command_.delta.yaw = cursorDelta.x * sensitivityX;
    command_.delta.pitch = cursorDelta.y * sensitivityY;

    cursorPosPrev_ = cursorPos;
}
