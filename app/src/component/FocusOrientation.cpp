#include "FocusOrientation.h"
#include <cmath>
#include <numbers>
#include <algorithm>


void FocusOrientation::Update(Tako::Transform& transform, float deltaTime)
{
    if (pTargetPosition_ == nullptr)
        return;

    constexpr float kPi = std::numbers::pi_v<float>;
    constexpr float kTwoPi = kPi * 2.0f;

    // 目標方向
    Tako::Vector3 diff = *pTargetPosition_ - transform.translate;

    // ターゲットと同じ位置なら何もしない
    if (diff.LengthSquared() < 1e-6f)
        return;

    Tako::Vector3 direction = diff.Normalize();

    // 目標回転
    Tako::Vector3 targetRotation = transform.rotate;
    targetRotation.x = std::atan2f(
        direction.y,
        std::sqrt(direction.x * direction.x + direction.z * direction.z));
    targetRotation.y = std::atan2f(direction.x, direction.z);

    // 角度差を -π ～ π に正規化
    auto WrapAngle = [](float angle)
    {
        constexpr float kPi = std::numbers::pi_v<float>;
        constexpr float kTwoPi = kPi * 2.0f;

        while (angle > kPi)
            angle -= kTwoPi;
        while (angle < -kPi)
            angle += kTwoPi;

        return angle;
    };

    float pitchDiff = WrapAngle(targetRotation.x - transform.rotate.x);
    float yawDiff = WrapAngle(targetRotation.y - transform.rotate.y);

    // 補間係数
    float t = std::clamp(rotateRatioPerSec_ * deltaTime, 0.0f, 1.0f);

    transform.rotate.x += pitchDiff * t;
    transform.rotate.y += yawDiff * t;

    // Rollを使わないなら固定
    transform.rotate.z = 0.0f;
}
