#pragma once
#include <component/PhysicsMovement.h>
#include <Vector3.h>
#include <Transform.h>
#include "PlayerInput.h"

/// <summary>
/// プレイヤーの移動処理 実装クラス
/// </summary>
class PlayerMovement : public PhysicsMovement
{
public:
    PlayerMovement(PlayerInput* pInput) : pInput_(pInput) {}
    ~PlayerMovement() = default;

    void Update(Tako::Transform& transform, float deltaTime) override;

    bool IsMove(float speedThreshold = 0.001f) const;
    void SetMovePower(float power) { movePower_ = power; }
    void SetJumpPower(float power) { jumpPower_ = power; }

private:
    void UpdateByInput(float dt);

    PlayerInput*    pInput_     = nullptr;
    float           movePower_  = 25.0f;
    float           jumpPower_  = 8.0f;
};