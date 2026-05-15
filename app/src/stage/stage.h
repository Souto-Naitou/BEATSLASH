#pragma once

#include "StageData.h"

#include <Object3d.h>
#include <AABBCollider.h>

#include <string>
#include <memory>


class Stage
{
public:

    void Initialize(const std::string& filePath ="");

    void Update(float deltaTime);

    void Draw();

    const StageData& GetStageData() const { return stageData_; }

private:

    StageData stageData_;


    std::unique_ptr<Tako::Object3d> model_;
    Tako::Transform transform_; 

    std::unique_ptr<Tako::AABBCollider> collider_; // ステージの当たり判定用コライダー
};