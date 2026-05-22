#pragma once
#include <component/PhysicsMovement.h>
#include <Vector3.h>
#include <Transform.h>
#include "PlayerInput.h"
#include "FollowCamera.h"

/// <summary>
/// プレイヤーの移動処理 実装クラス
/// </summary>
class PlayerMovement : public PhysicsMovement
{
public:
    PlayerMovement(PlayerInput* pInput, FollowCamera& followCamera) : pInput_(pInput), followCamera_(followCamera) {}
    ~PlayerMovement() = default;

    void Update(Tako::Transform& transform, float deltaTime) override;

    bool IsMove(float speedThreshold = 0.001f) const;
    void SetMovePower(float power) { movePower_ = power; }
    void SetJumpPower(float power) { jumpPower_ = power; }
    Tako::Vector3 GetMoveDirection() const { return moveDirection_; }

private:
    void UpdateByInput(float dt);
    void UpdateByCamera(float dt);

    Tako::Vector3   moveDirection_ = {};
    PlayerInput*    pInput_     = nullptr;
    FollowCamera&   followCamera_;
    float           movePower_  = 25.0f;
    float           jumpPower_  = 8.0f;
};