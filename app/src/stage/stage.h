#pragma once

#include "StageData.h"

#include <Object3d.h>
#include <AABBCollider.h>

#include <string>
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

private:

    StageData stageData_;


    std::unique_ptr<Tako::Object3d> model_;
    Tako::Transform transform_; 

    std::unique_ptr<Tako::Object3d> door_;

    std::unique_ptr<Tako::AABBCollider> collider_; // ステージの当たり判定用コライダー
};