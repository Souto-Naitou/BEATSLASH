#pragma once
#include <EmitterManager.h>
#include <string>

class ParryPresentation
{
public:
    ParryPresentation(Tako::EmitterManager& emitterManager);
    void Play(const Tako::Vector3& position);
    void Update();

private:
    uint32_t uniqueIndex_ = 0; // エミッターの一意なインデックス生成用
    Tako::EmitterManager& emitterManager_;
    std::string uniqueNameParryEffect_;
};