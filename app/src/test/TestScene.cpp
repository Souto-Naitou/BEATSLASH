#include "TestScene.h"

#include "Draw2D.h"
#include "GPUParticle.h"
#include "Input.h"
#include "Object3dBasic.h"
#include "SceneManager.h"
#include "SpriteBasic.h"
#include "utility/CollisionUtility.h"
#include <CollisionManager.h>
#include <cassert>
#include <cmath>
#ifdef _DEBUG
#include <ImGui.h>
#include <DebugCamera.h>
#endif

using namespace Tako;

void TestScene::Initialize()
{
#ifdef _DEBUG
    DebugCamera::GetInstance()->Initialize();
    Object3dBasic::GetInstance()->SetDebug(false);
    Draw2D::GetInstance()->SetDebug(false);
    GPUParticle::GetInstance()->SetIsDebug(false);
#endif

    /// ================================== ///
    ///              初期化処理              ///
    /// ================================== ///

    //// CollisionUtility テスト（起動時1回だけ実行）
    //{
    //    using namespace CollisionUtility;
    //    auto approx = [](float a, float b) { return std::abs(a - b) < 0.001f; };

    //    // Sphere vs Sphere: X軸方向に 0.5 めり込み
    //    // A=(0,0,0) r=1, B=(1.5,0,0) r=1 → A は -X 方向に押し戻される
    //    {
    //        Tako::SphereCollider a, b;
    //        a.SetOffset({0.0f, 0.0f, 0.0f}); a.SetRadius(1.0f);
    //        b.SetOffset({1.5f, 0.0f, 0.0f}); b.SetRadius(1.0f);
    //        auto info = CalcCollisionInfo(&a, &b);
    //        assert(approx(info.penetration, 0.5f));
    //        assert(approx(info.contactNormal.x, -1.0f));
    //        assert(approx(info.contactNormal.y,  0.0f));
    //        assert(approx(info.contactNormal.z,  0.0f));
    //    }

    //    // AABB vs AABB: X軸方向に 0.5 めり込み
    //    // A=center(0,0,0) size(2,2,2), B=center(1.5,0,0) size(2,2,2)
    //    {
    //        Tako::AABBCollider a, b;
    //        a.SetOffset({0.0f, 0.0f, 0.0f}); a.SetSize({2.0f, 2.0f, 2.0f});
    //        b.SetOffset({1.5f, 0.0f, 0.0f}); b.SetSize({2.0f, 2.0f, 2.0f});
    //        auto info = CalcCollisionInfo(&a, &b);
    //        assert(approx(info.penetration, 0.5f));
    //        assert(approx(info.contactNormal.x, -1.0f));
    //        assert(approx(info.contactNormal.y,  0.0f));
    //        assert(approx(info.contactNormal.z,  0.0f));
    //    }

    //    // Sphere vs AABB: 球が AABB の右面から 0.5 めり込み
    //    // sphere=(1.5,0,0) r=1, AABB=center(0,0,0) size(2,2,2)
    //    {
    //        Tako::SphereCollider sphere;
    //        Tako::AABBCollider aabb;
    //        sphere.SetOffset({1.5f, 0.0f, 0.0f}); sphere.SetRadius(1.0f);
    //        aabb.SetOffset({0.0f, 0.0f, 0.0f});   aabb.SetSize({2.0f, 2.0f, 2.0f});
    //        auto info = CalcCollisionInfo(&sphere, &aabb);
    //        assert(approx(info.penetration, 0.5f));
    //        assert(approx(info.contactNormal.x, 1.0f));
    //    }

    //    // CalcPushback 対称ケース: AABB が self, Sphere が other
    //    // AABB=center(0,0,0) size(2,2,2), sphere=(1.5,0,0) r=1
    //    // AABB は -X 方向に 0.5 押し戻される
    //    {
    //        Tako::AABBCollider aabb;
    //        Tako::SphereCollider sphere;
    //        aabb.SetOffset({0.0f, 0.0f, 0.0f}); aabb.SetSize({2.0f, 2.0f, 2.0f});
    //        sphere.SetOffset({1.5f, 0.0f, 0.0f}); sphere.SetRadius(1.0f);
    //        auto pushback = CalcPushback(&aabb, &sphere);
    //        assert(approx(pushback.x, -0.5f));
    //        assert(approx(pushback.y,  0.0f));
    //        assert(approx(pushback.z,  0.0f));
    //    }
    //}


    testModel1_ = std::make_unique<Object3d>();
    testModel1_->Initialize();
    testModel1_->SetModel("white_cube.gltf");
    testModel1_->SetTranslate({ -2.4f, 0.0f, 0.0f });
    testModel1_->SetMaterialColor({ 1,1,1,1 });
    testModel1_->SetEnableLighting(false);
    testModel1Transform_ = testModel1_->GetTransform();

    testModel2_ = std::make_unique<Object3d>();
    testModel2_->Initialize();
    testModel2_->SetModel("white_cube.gltf");
    testModel2_->SetTranslate({ 2.4f, 1.0f, 0.0f });
    testModel2_->SetMaterialColor({ 1,0,0,1 });
    testModel2_->SetEnableLighting(false);
    testModel2Transform_ = testModel2_->GetTransform();

    aabbCollider1_ = std::make_unique<TestCollider>();
    aabbCollider1_->SetSize({ 3.0f, 3.0f, 3.0f });
    aabbCollider1_->SetOwner(testModel1_.get());
    aabbCollider1_->SetTypeID(1); // enum 面倒なのでハードコードで型IDを設定
    aabbCollider1_->SetTransform(&testModel1Transform_);

    aabbCollider2_ = std::make_unique<SphereCollider>();
    aabbCollider2_->SetRadius(1.5f);
    //aabbCollider2_->SetSize({ 3.0f, 3.0f, 3.0f });
    aabbCollider2_->SetOwner(testModel2_.get());
    aabbCollider2_->SetTypeID(2); // enum 面倒なのでハードコードで型IDを設定
    aabbCollider2_->SetTransform(&testModel2Transform_);

    CollisionManager::GetInstance()->AddCollider(aabbCollider1_.get());
    CollisionManager::GetInstance()->AddCollider(aabbCollider2_.get());

    CollisionManager::GetInstance()->SetDebugDrawEnabled(true);
    CollisionManager::GetInstance()->SetCollisionMask(1, 2, true);
    // ライトの設定
    Object3dBasic* obj3d = Object3dBasic::GetInstance();
    obj3d->SetDirectionalLight(
        { 1.0f, -1.0f, 1.0f },   // 方向
        { 1.0f, 1.0f, 1.0f, 1.0f }, // 白色
        0,
        1.0f                      // 強度
    );
    obj3d->SetSceneCenter(Vector3(0.0f, 0.0f, 0.0f));  // デフォルト値
    obj3d->SetAutoUpdatePosition(true);  // デフォルト値

}


void TestScene::Finalize()
{

}

void TestScene::Update()
{
    /// ================================== ///
    ///              更新処理               ///
    /// ================================== ///

    const float delta = 0.016f; // 仮のフレーム時間（60FPS想定）
    const float spd = 2.0f; // 移動速度
    const float moveAmount = spd * delta;
    if (Input::GetInstance()->PushKey(DIK_A))
    {
        testModel1Transform_.translate += Vector3(-1.0f, 0.0f, 0.0f) * moveAmount;
    }
    if (Input::GetInstance()->PushKey(DIK_D))
    {
        testModel1Transform_.translate += Vector3(1.0f, 0.0f, 0.0f) * moveAmount;
    }
    if (Input::GetInstance()->PushKey(DIK_W))
    {
        testModel1Transform_.translate += Vector3(0.0f, 1.0f, 0.0f) * moveAmount;
    }
    if (Input::GetInstance()->PushKey(DIK_S))
    {
        testModel1Transform_.translate += Vector3(0.0f, -1.0f, 0.0f) * moveAmount;
    }

    testModel1_->SetTranslate(testModel1Transform_.translate);
    testModel2_->SetTranslate(testModel2Transform_.translate);

    testModel1_->Update();
    testModel2_->Update();

    CollisionManager::GetInstance()->CheckAllCollisions();

    Object3dBasic::GetInstance()->SetDirectionalLight(
        { 1.0f, -1.0f, 1.0f },   // 方向
        { 1.0f, 1.0f, 1.0f, 1.0f }, // 白色
        0,
        1.0f                      // 強度
    );

    if (Input::GetInstance()->TriggerKey(DIK_RETURN))
    {
        SceneManager::GetInstance()->ChangeScene("");
    }
}

void TestScene::Draw()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///
    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();




    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();

    testModel1_->Draw();
    testModel2_->Draw();


    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();



}

void TestScene::DrawWithoutEffect()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///

    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();




    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();





    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();



}

void TestScene::DrawImGui()
{
#ifdef _DEBUG

    /// ================================== ///
    ///             ImGuiの描画              ///
    /// ================================== ///


#endif // _DEBUG
}