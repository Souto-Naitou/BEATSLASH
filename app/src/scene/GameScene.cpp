#include "GameScene.h"

#include "Draw2D.h"
#include "GPUParticle.h"
#include "Input.h"
#include "Object3dBasic.h"
#include "SceneManager.h"
#include "SpriteBasic.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif
#include <FrameTimer.h>
#include <ShadowRenderer.h>
#include <CollisionManager.h>

using namespace Tako;

void GameScene::Initialize()
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

    stage_ = std::make_unique<StageSequence>();
    stage_->Initialize();


    Object3dBasic* obj3d = Object3dBasic::GetInstance();
    obj3d->SetDirectionalLight(
        { 0.0f, -1.0f, 1.0f },   // 方向
        { 1.0f, 1.0f, 1.0f, 1.0f }, // 白色
        1,
        1.0f                      // 強度
    );
    //obj3d->SetSceneCenter(Vector3(0.0f, 0.0f, 0.0f));  // デフォルト値
    obj3d->SetAutoUpdatePosition(true);  // デフォルト値

    Tako::ShadowRenderer::GetInstance()->SetEnabled(false);
    Tako::CollisionManager::GetInstance()->SetDebugDrawEnabled(true);
}


void GameScene::Finalize()
{

}

void GameScene::Update()
{
    /// ================================== ///
    ///              更新処理               ///
    /// ================================== ///

    const float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();

    stage_->Update(deltaTime);

    if (Input::GetInstance()->TriggerKey(DIK_RETURN))
    {
        stage_->NotifyClear();
        //SceneManager::GetInstance()->ChangeScene("");
    }
    CollisionManager::GetInstance()->CheckAllCollisions();
}

void GameScene::Draw()
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


    stage_->Draw();

    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();

    Tako::CollisionManager::GetInstance()->DrawColliders();


}

void GameScene::DrawWithoutEffect()
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

void GameScene::DrawImGui()
{
#ifdef _DEBUG

    /// ================================== ///
    ///             ImGuiの描画              ///
    /// ================================== ///


#endif // _DEBUG
}