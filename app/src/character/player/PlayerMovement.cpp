#include "PlayerMovement.h"
#include <cassert>
#include <algorithm>


void PlayerMovement::Update(Tako::Transform& transform, float deltaTime)
{
    // 入力による加速度の更新
    this->UpdateByInput(deltaTime);

    // 位置の更新 (加速度と速度から算出)
    PhysicsMovement::PositionUpdate(transform.translate, deltaTime);
}

bool PlayerMovement::IsMove(float speedThreshold /*= 0.0f*/) const
{
    const float speedSq = PhysicsMovement::GetVelocity().LengthSquared();
    const float thresholdSq = speedThreshold * speedThreshold;
    return speedSq > thresholdSq;
}

void PlayerMovement::UpdateByInput(float dt)
{
    const auto& inputData = pInput_->GetCommand();
    const auto& move = inputData.move;
    const auto& cameraRotation = followCamera_.GetRotation();
    const float yaw = cameraRotation.y;

    Tako::Vector3 forward = Tako::Vector3(std::sin(yaw), 0.0f, std::cos(yaw));
    Tako::Vector3 right = Tako::Vector3(forward.z, 0.0f, -forward.x);
    moveDirection_ = forward * move.z + right * move.x;

    if (moveDirection_.LengthSquared() > 0.0f) { moveDirection_.Normalize(); }

    ApplyForce(moveDirection_ * movePower_);
    
    if (inputData.isJumpTriggered)
    {
        AddImpulse(Tako::Vector3(0.0f, jumpPower_, 0.0f));
    }
}

void PlayerMovement::UpdateByCamera(float dt)
{
    
    
}
