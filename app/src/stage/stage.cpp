#include "stage.h"
#include <CollisionManager.h>
#include <Model.h>
#include <type/ColliderTypeID.h>
Stage::~Stage()
{
    for (auto& col : colliders_)
        Tako::CollisionManager::GetInstance()->RemoveCollider(col.get());
}

void Stage::Initialize(const StageData& stageData)
{
    // 既存コライダーを CollisionManager から除去
    for (auto& col : colliders_)
        Tako::CollisionManager::GetInstance()->RemoveCollider(col.get());
    colliders_.clear();

    stageData_ = stageData; // ステージデータを保存（先に行う：コライダーのTransform*がstageData_を参照するため）

    const float cubeDefaultSize = 3.0f;

    // 床モデル＋コライダーを floors の数だけ生成
    models_.clear();
    for (size_t i = 0; i < stageData_.floorTransform.size(); ++i)
    {
        const auto& floorTf = stageData_.floorTransform[i];

        auto model = std::make_unique<Tako::Object3d>();
        model->Initialize();
        model->SetModel("white_cube.gltf");
        model->SetMaterialColor({ 1,1,1,1 });
        model->SetEnableLighting(true);
        model->SetTransform(floorTf);
        models_.push_back(std::move(model));

        auto col = std::make_unique<Tako::OBBCollider>();
        col->SetSize(floorTf.scale * cubeDefaultSize);
        col->SetOwner(this);
        col->SetTypeID(static_cast<int32_t>(ColliderTypeID::Terrain));
        col->SetTransform(&stageData_.floorTransform[i]); // stageData_ のアドレスを参照
        Tako::CollisionManager::GetInstance()->AddCollider(col.get());
        colliders_.push_back(std::move(col));
    }

    Tako::CollisionManager::GetInstance()->SetCollisionMask(1, 200, true);

    // ドアの初期化
    door_ = std::make_unique<Tako::Object3d>();
    door_->Initialize();
    door_->SetModel("Door/Door.gltf");
    door_->SetMaterialColor({ 0.5f,0.3f,0.2f,1.0f });
    door_->SetEnableLighting(true);
    door_->SetTransform(stageData.doorTransform);

    door_->GetModel()->SetAnimation("OpenAnim");
    door_->GetModel()->SetAnimationLoop("OpenAnim", false);
    door_->GetModel()->PauseAnimation();
}

void Stage::Update(float deltaTime)
{
    for (size_t i = 0; i < models_.size(); ++i)
    {
        models_[i]->SetTransform(stageData_.floorTransform[i]);
        models_[i]->Update();
    }

    door_->Update();
}

void Stage::Draw()
{
    for (auto& model : models_)
    {
        model->Draw();
    }

    door_->Draw();
}

void Stage::OpenDoor()
{
    door_->GetModel()->ResumeAnimation();
}
