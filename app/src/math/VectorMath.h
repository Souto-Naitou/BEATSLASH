#pragma once
#include <cmath>
#include <Vector3.h>

inline float VectorToAngle(const Tako::Vector3& vec)
{
    float angle = std::atan2(vec.x, vec.z);
    return angle;
}