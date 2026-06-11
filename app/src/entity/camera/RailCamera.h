#pragma once
#include <memory>
#include <Vector3.h>
#include <presentation/animation/AnimationTimeline.hpp>
#include <Camera.h>
#include <component/FocusOrientation.h>
#include "debug/GameParameter.h"

class RailCamera
{
public:
    void Initialize(const Tako::Vector3* pCenter);
    void Update(AnimationTimeline<Tako::Vector3>& rail, float deltaTime);

    void SetFocusTarget(const Tako::Vector3* pCenter) { pFocusOrientation_->SetTargetPosition(pCenter); }
    Tako::Camera* GetCamera() const { return pCamera_.get(); }

private:
    std::unique_ptr<Tako::Camera> pCamera_ = nullptr;
    std::unique_ptr<FocusOrientation> pFocusOrientation_;     // !< 注視制御クラスのインスタンス

    EnableDebug("RailCamera");
    GameParameter(Tako::Vector3, kRotate_, Tako::Vector3(0.0f, 20.0f, 0.0f));
};