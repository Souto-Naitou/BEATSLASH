#pragma once

#include "StageData.h"

#include <Object3d.h>
#include <OBBCollider.h>

#include <memory>
#include <functional>


class Stage
{
public:

    ~Stage();

    void Initialize(const StageData& stageData);

    void Update(float deltaTime);

    void Draw();

    const StageData& GetStageData() const { return stageData_; }

    void OpenDoor();

    void SetOnDoorOpenFinished(std::function<void()> cb) { onDoorOpenFinished_ = std::move(cb); }

    void CollisionActive(bool active) { for (auto& col : colliders_) col->SetActive(active); }

private:

    StageData stageData_;

    std::vector<std::unique_ptr<Tako::Object3d>> models_;
    std::vector<std::unique_ptr<Tako::OBBCollider>> colliders_;

    std::unique_ptr<Tako::Object3d> door_;
    bool isDoorOpening_ = false;

    std::function<void()> onDoorOpenFinished_;

};