#pragma once

#include "StageData.h"

#include <Object3d.h>
#include <OBBCollider.h>

#include <memory>


class Stage
{
public:

    ~Stage();

    void Initialize(const StageData& stageData);

    void Update(float deltaTime);

    void Draw();

    const StageData& GetStageData() const { return stageData_; }

    void OpenDoor();

    void CollisionActive(bool active) { for (auto& col : colliders_) col->SetActive(active); }

private:

    StageData stageData_;

    std::vector<std::unique_ptr<Tako::Object3d>> models_;
    std::vector<std::unique_ptr<Tako::OBBCollider>> colliders_; // 床ごとのコライダー

    std::unique_ptr<Tako::Object3d> door_;

};