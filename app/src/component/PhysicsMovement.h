#pragma once
#include <component/IMovement.h>
#include <Transform.h>
#include <Vector3.h>

class PhysicsMovement : public IMovement
{
public:
    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update(Tako::Transform& transform, float deltaTime) override;

    /// <summary>
    /// 指定された力を加速度に適用します。
    /// </summary>
    /// <param name="force">適用する力のベクトル。</param>
    void ApplyForce(const Tako::Vector3& force) { acceleration_ += force; }

    /// <summary>
    /// 速度をゼロにリセットします。
    /// </summary>
    void ResetVelocity() { velocity_ = Tako::Vector3(); }

    /// <summary>
    /// Y軸方向の速度をゼロにリセットします。ジャンプのリセットなどに使用します。
    /// </summary>
    /// <returns></returns>
    void ResetVelocityY() { velocity_.y = 0.0f; }

    /// <summary>
    /// 速度に衝撃を加えます。
    /// 瞬間的に速度を変化させたい場合に使用します。
    /// </summary>
    /// <param name="impulse">加える衝撃ベクトル。</param>
    void AddImpulse(const Tako::Vector3& impulse);

    /// <summary>
    /// 移動を停止します。演出中などに使用します。
    /// </summary>
    void StopMovement();

    /// <summary>
    /// 摩擦を適用して速度を調整します。
    /// </summary>
    /// <param name="friction">速度に乗算する摩擦係数。</param>
    void ApplyFriction(float friction)
    { 
        velocity_.x *= friction; 
        velocity_.z *= friction; 
    }

    /// <summary>
    /// 重力を適用して速度を調整します。
    /// </summary>
    /// <param name="gravity">重力</param>
    /// <param name="mass">質量</param>
    /// <param name="deltaTime">デルタタイム</param>
    void ApplyGravity(float mass, float deltaTime);

    /// [ Getters ]
    Tako::Vector3 GetVelocity() const { return velocity_; }
    Tako::Vector3 GetAcceleration() const { return acceleration_; }

protected:
    /// <summary>
    /// フレーム時間に基づいて位置を更新します。
    /// 毎フレーム呼び出すことを想定しています。
    /// </summary>
    /// <param name="position">更新される位置ベクトル。</param>
    /// <param name="deltaTime">前回の更新からの経過時間。</param>
    void PositionUpdate(Tako::Vector3& position, float deltaTime);

private:
    static constexpr float kGravity_ = 9.8f; // 重力加速度の定数値

    Tako::Vector3 acceleration_ = {};
    Tako::Vector3 velocity_ = {};
};