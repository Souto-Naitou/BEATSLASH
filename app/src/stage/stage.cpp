#include "stage.h"
#include <CollisionManager.h>
#include <Model.h>
#include <type/ColliderTypeID.h>
#include <math/Color.h>


Stage::~Stage()
{
    for (auto& col : colliders_)
        Tako::CollisionManager::GetInstance()->RemoveCollider(col.get());
}

void Stage::Initialize(const StageData& stageData)
{
#ifdef DEBUG


    kStageColor_.SetOnChange([this](const RGBA& newColor)
    {
        for (auto& model : models_)
        {
            model->SetMaterialColor(newColor.to_Vector4());
        }
    });
#endif // DEBUG

    // 既存コライダーを CollisionManager から除去
    for (auto& col : colliders_)
        Tako::CollisionManager::GetInstance()->RemoveCollider(col.get());
    colliders_.clear();
    wallTransforms_.clear();

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
        model->SetMaterialColor(kStageColor_->to_Vector4());
        model->SetEnableLighting(true);
        model->SetEnableHighlight(false);
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

    // 床の周囲を見えない壁で囲む（場外への移動防止）
    if (stageData_.surroundWalls && !stageData_.floorTransform.empty())
    {
        CreateSurroundingWalls(stageData_.floorTransform[0], cubeDefaultSize);
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

    if (isDoorOpening_ && door_->GetModel()->IsAnimationFinished("OpenAnim"))
    {
        isDoorOpening_ = false;
        if (onDoorOpenFinished_) onDoorOpenFinished_();
    }
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
    isDoorOpening_ = true;
    door_->GetModel()->ResumeAnimation();
}

void Stage::CreateSurroundingWalls(const Tako::Transform& floorTf, float cubeDefaultSize)
{
    constexpr float kWallHeight = 30.0f;
    constexpr float kWallThickness = 3.0f;

    const float width = floorTf.scale.x * cubeDefaultSize;
    const float depth = floorTf.scale.z * cubeDefaultSize;
    const Tako::Vector3& center = floorTf.translate;
    const float wallY = center.y + kWallHeight * 0.5f;

    struct WallDef
    {
        Tako::Vector3 translate;
        Tako::Vector3 size;
    };
    // X方向の壁は奥行きを厚さ2枚分延長して四隅の隙間を防ぐ
    const WallDef walls[4] = {
        { { center.x + (width + kWallThickness) * 0.5f, wallY, center.z }, { kWallThickness, kWallHeight, depth + kWallThickness * 2.0f } },
        { { center.x - (width + kWallThickness) * 0.5f, wallY, center.z }, { kWallThickness, kWallHeight, depth + kWallThickness * 2.0f } },
        { { center.x, wallY, center.z + (depth + kWallThickness) * 0.5f }, { width + kWallThickness * 2.0f, kWallHeight, kWallThickness } },
        { { center.x, wallY, center.z - (depth + kWallThickness) * 0.5f }, { width + kWallThickness * 2.0f, kWallHeight, kWallThickness } },
    };

    // コライダーはTransformのアドレスを参照し続けるため、再配置が起きないよう先に確保する
    wallTransforms_.reserve(4);

    for (const auto& wall : walls)
    {
        Tako::Transform tf;
        tf.translate = wall.translate;
        tf.rotate = { 0.0f, 0.0f, 0.0f };
        tf.scale = { 1.0f, 1.0f, 1.0f };
        wallTransforms_.push_back(tf);

        auto col = std::make_unique<Tako::OBBCollider>();
        col->SetSize(wall.size);
        col->SetOwner(this);
        col->SetTypeID(static_cast<int32_t>(ColliderTypeID::Terrain));
        col->SetTransform(&wallTransforms_.back());
        Tako::CollisionManager::GetInstance()->AddCollider(col.get());
        colliders_.push_back(std::move(col));
    }
}
