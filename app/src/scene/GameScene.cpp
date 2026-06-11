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
#include <DebugUIManager.h>
#endif
#include <FrameTimer.h>
#include <ShadowRenderer.h>
#include <CollisionManager.h>
#include <ozSound/audio/SoundEngine.h>
#include <common/ParticleEmitterPresetNames.h>
#include <PostEffectManager.h>


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

    ShadowRenderer::GetInstance()->SetMaxShadowDistance(50.0f);

    // 画像の読み込み
    this->LoadImageAll();

    // ポストエフェクトの初期化と適用
    this->ApplyPostEffects();

    /// ステージの初期化
    pStage_ = std::make_unique<StageSequence>();
    pStage_->Initialize("resources/stage/StageData.json");

    /// カメラの初期化
    pCameraDirector_ = std::make_unique<CameraDirector>();
    pCameraDirector_->Initialize();

    pStage_->SetOnDoorOpened([this](const Tako::Transform& doorTransform)
    {
        pCameraDirector_->StartFocus(doorTransform, 1.0f);
    });

    pCameraDirector_->SetOnFocusArrived([this]()
    {
        pStage_->OpenCurrentDoor();
    });

    pStage_->SetOnDoorOpenFinished([this]()
    {
        pCameraDirector_->NotifyDoorOpenFinished();
    });

    const float BPM = 150.0f;
    pInputTimingJudge_ = std::make_unique<InputTimingJudge>();
    pInputTimingJudge_->Initialize(BPM, 0.2f, 0.4f);

    pBeatClock_ = std::make_unique<BeatClock>();
    pBeatClock_->Initialize(BPM, 0.55f);

    /// コンボシステムと入力判定クラスの初期化
    pComboSystem_ = std::make_unique<ComboSystem>();
    pComboBuffSystem_ = std::make_unique<ComboBuffSystem>(pComboSystem_.get(), pInputTimingJudge_.get(), pBeatClock_.get());

    /// エミッターマネージャの初期化
    pEmitterManager_ = std::make_unique<Tako::EmitterManager>(Tako::GPUParticle::GetInstance());
#ifdef _DEBUG
    Tako::DebugUIManager::GetInstance()->SetEmitterManager(pEmitterManager_.get());
#endif // _DEBUG

    this->LoadParticleEmitterPresets();

    /// プレイヤー攻撃ファクトリーの初期化
    /// プレイヤーモデルの参照が必要なため、プレイヤー初期化後に生成する必要がある
    PlayerAttackFactory::Dependencies playerAttackFactoryDeps
    {
        .comboBuffSystem = *pComboBuffSystem_,
        .colliderRepository = colliderRepository_,
        .emitterManager = *pEmitterManager_,
    };
    pPlayerAttackFactory_ = std::make_unique<PlayerAttackFactory>(playerAttackFactoryDeps);

    /// コライダーリポジトリと攻撃リポジトリの初期化
    AttackRepository::FactoryDependencies attackRepoDeps
    {
        .pPlayerAttackFactory = pPlayerAttackFactory_.get()
    };
    pAttackRepository_ = std::make_unique<AttackRepository>(attackRepoDeps);

    /// プレイヤーの初期化
    Player::InitData playerInitData
    {
        *pAttackRepository_,
        *pCameraDirector_->GetFollowCamera(),
        *pComboBuffSystem_,
        *pBeatClock_,
        *pEmitterManager_
    };
    pPlayer_ = std::make_unique<Player>(playerInitData);
    pPlayer_->Initialize();
    pCameraDirector_->SetFollowTarget(&pPlayer_->GetTransform());
    pStage_->SetOnStageChanged([this](const Tako::Transform& spawnTransform)
                               {
                                   pPlayer_->Respawn(spawnTransform);
                               });

    // 敵の初期化
    pEnemyManager_ = std::make_unique<EnemyManager>(pPlayer_.get(), pBeatClock_.get(), pEmitterManager_.get());
    // テスト用にステージ0に敵をスポーン
    pEnemyManager_->SpawnEnemy(0, Tako::Vector3{ 0.0f, 150.0f, 0.0f });

    pGameHUD_ = std::make_unique<GameHUD>(*pComboBuffSystem_, *pBeatClock_,
                                          pPlayer_->GetHPComponent(), pPlayer_->GetPlayerInput());
    pGameHUD_->Initialize();

    Object3dBasic* obj3d = Object3dBasic::GetInstance();
    obj3d->SetDirectionalLight(
        { 0.0f, -1.0f, 1.0f },   // 方向
        { 1.0f, 1.0f, 1.0f, 1.0f }, // 白色
        1,
        1.0f                      // 強度
    );

    obj3d->SetAutoUpdatePosition(true);  // デフォルト値

    Tako::ShadowRenderer::GetInstance()->SetEnabled(false);
    Tako::CollisionManager::GetInstance()->SetDebugDrawEnabled(true);

    pBeatClock_->SetMusicSoundHandle(ozSound::SoundEngine::GetInstance()->Play("bgm_game_0", 0.2f, true));
    pBeatClock_->Start();
}


void GameScene::Finalize()
{
    pPlayer_->Finalize();
}

void GameScene::Update()
{
    /// ================================== ///
    ///              更新処理               ///
    /// ================================== ///

    const float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();

    // ステージの更新
    pStage_->Update(deltaTime);
    // プレイヤーの更新
    pPlayer_->Update();
    // 敵の更新
    pEnemyManager_->Update(pStage_->GetCurrentIndex());


    pBeatClock_->Update();
    pAttackRepository_->Update();
    
    // 非アクティブの攻撃を削除
    pAttackRepository_->EraseInactiveAttacks();
    // 非アクティブのコライダーを削除
    colliderRepository_.EraseInactiveColliders();

    pCameraDirector_->Update(deltaTime);
    pGameHUD_->Update();

    if (pEnemyManager_->IsEmpty(pStage_->GetCurrentIndex()))
    {
        // TODO：敵が全部死んだらこいつを呼ぶ
        pStage_->NotifyClear();
    }
    pEmitterManager_->Update();
    CollisionManager::GetInstance()->CheckAllCollisions();
}

void GameScene::Draw()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///
    auto pShadowRenderer = Tako::ShadowRenderer::GetInstance();
    if (pShadowRenderer->IsEnabled())
    {
        Tako::ShadowRenderer::GetInstance()->BeginShadowPass();
        this->DrawObjects();
        Tako::ShadowRenderer::GetInstance()->EndShadowPass();
    }

    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();




    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();
    this->DrawObjects();

    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();
    pGameHUD_->Draw();

    pStage_->DrawTransition();


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

void GameScene::LoadParticleEmitterPresets()
{
    pEmitterManager_->LoadPreset(Global::ParticleEmitterPresetNames::kTrail);
    pEmitterManager_->LoadPreset(Global::ParticleEmitterPresetNames::kTrailBlackBlue);
    pEmitterManager_->LoadPreset(Global::ParticleEmitterPresetNames::kShort);
    pEmitterManager_->LoadPreset(Global::ParticleEmitterPresetNames::kParrySuccess);
}

void GameScene::LoadImageAll()
{
    auto tm = Tako::TextureManager::GetInstance();
    for (const auto& entry : std::filesystem::recursive_directory_iterator("resources/Texture"))
    {
        if (entry.is_regular_file())
        {
            auto newPath = std::filesystem::relative(entry.path(), "resources/Texture");
            tm->LoadTexture(newPath.string());
        }
    }
}

void GameScene::DrawObjects()
{
    pStage_->Draw();
    pPlayer_->Draw();
    pEnemyManager_->Draw(pStage_->GetCurrentIndex());
}

void GameScene::ApplyPostEffects()
{
    Tako::PostEffectManager::GetInstance()->AddEffectToChain("DepthBasedOutline");
}
