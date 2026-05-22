#pragma once
#include <Input.h>
#include <Vector2.h>
#include <debug/GameParameter.h>

class CameraInput
{
public:
    struct Command
    {
        struct Delta
        {
            float pitch; // カメラの上下回転
            float yaw;   // カメラの左右回転
        } delta;
    };

    void Update();
    Command GetCommand() const { return command_; }

private:
    EnableDebug("Camera Input");
    GameParameter(float, sensitivityX, 0.01f);
    GameParameter(float, sensitivityY, 0.01f);

    Command command_ = {};
    Tako::Vector2 cursorPosPrev_ = {};
    Tako::Input* pInput_ = Tako::Input::GetInstance();
};