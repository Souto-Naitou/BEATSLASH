#pragma once
#include <Vector3.h>
#include <Transform.h>

class FocusOrientation
{
public:
    void SetTargetPosition(const Tako::Vector3* pPosition) { pTargetPosition_ = pPosition; }
    void SetRotateRatio(float ratioPerSec) { rotateRatioPerSec_ = ratioPerSec; }
    void Update(Tako::Transform& transform, float deltaTime);

private:
    float rotateRatioPerSec_ = 1.0f;
    const Tako::Vector3* pTargetPosition_ = nullptr;
};