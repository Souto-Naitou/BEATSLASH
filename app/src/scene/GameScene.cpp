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
#include <character/boss/bt/BossBTNodeRegistration.h>
#include <PostEffectManager.h>
#include <math/Easing.h>


using namespace Tako;

namespace
{
    float EaseInOutQuint(float x)
    {
        if (x < 0.5f)
        {
            return 16.0f * x * x * x * x * x;
        }
        else
        {
            float val = -2.0f * x + 2.0f;
            return 1.0f - (val * val * val * val * val) / 2.0f;
        }
    }
}

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

    /// エミッターマネージャの初期化
    pEmitterManager_ = std::make_unique<Tako::EmitterManager>(Tako::GPUParticle::GetInstance());
	
    /// エフェクトのプリセットの読み込み
    // ドアが開いているときのエフェクトをロード
    pEmitterManager_->LoadPreset("door_open");
    pEmitterManager_->SetEmitterActive("door_open", false);

	// 背景エフェクトをロード
	pEmitterManager_->LoadPreset("field_background");
	pEmitterManager_->SetEmitterActive("field_background", true);

    // ボス死亡時のエフェクト（敵スポーンのエフェクトを流用）
    pEmitterManager_->LoadPreset("boss_dead");
    pEmitterManager_->SetEmitterActive("boss_dead", false);

    /// ステージの初期化
    pStage_ = std::make_unique<StageSequence>();
    pStage_->Initialize("resources/stage/StageData.json");

    /// カメラの初期化
    pCameraDirector_ = std::make_unique<CameraDirector>();
    pCameraDirector_->Initialize();

    pStage_->SetOnDoorOpened([this](const Tako::Transform& doorTransform)
    {
        pCameraDirector_->StartFocus(doorTransform, 1.0f);
		
        // ドアが開いているときのエフェクトを再生
		pEmitterManager_->SetEmitterActive("door_open", true);
		pEmitterManager_->SetEmitterPosition("door_open", doorTransform.translate + Tako::Vector3( -3.5f, 4.5f, 0.0f ));
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
								   // ドアのエフェクトを非アクティブにする
								   pEmitterManager_->SetEmitterActive("door_open", false);

                                   pPlayer_->Respawn(spawnTransform);

                                   // ボスステージでのみボスを存在させる
                                   if (pStage_->GetCurrentStageData().spawnBoss)
                                   {
                                       if (!pBoss_)
                                       {
                                           SpawnBoss();
                                       }
                                   }
                                   else
                                   {
                                       pBoss_.reset();
                                   }
                               });

    // 敵の初期化
    pEnemyManager_ = std::make_unique<EnemyManager>(pPlayer_.get(), pBeatClock_.get(), pEmitterManager_.get());
    
    //　ステージ１の敵の配置
    pEnemyManager_->SpawnEnemy(0, Tako::Vector3(10.0f, 5.0f, 3.0f));
	pEnemyManager_->SpawnEnemy(0, Tako::Vector3(-13.0f, 5.0f, 3.0f));

	// ステージ２の敵の配置
    pEnemyManager_->SpawnEnemy(1, Tako::Vector3(-15.0f, 5.0f, -6.0f));
	pEnemyManager_->SpawnEnemy(1, Tako::Vector3(-15.0f, 7.0f, 17.0f));
    pEnemyManager_->SpawnEnemy(1, Tako::Vector3(15.0f, 3.0f, -7.0f));
    pEnemyManager_->SpawnEnemy(1, Tako::Vector3(-10.0f, 10.0f, -22.0f));

    // ボス用BTノードのファクトリ登録（一度だけでよい）
    RegisterBossBTNodes();

#ifdef _DEBUG
    // BTエディタの初期化（ボス不在時も使えるようボス生成とは独立して行う）
    Tako::EditorConfig btEditorConfig;
    btEditorConfig.btJsonDir = "resources/Json/BT/";
    btEditorConfig.initialTreeFile = "BossTree.json";
    pBtEditor_ = std::make_unique<Tako::BehaviorTreeEditor>();
    pBtEditor_->Initialize(btEditorConfig);
#endif

    HUDContext hudContext
    {
        .comboBuffSystem = *pComboBuffSystem_,
        .beatClock = *pBeatClock_,
        .playerHPComponent = pPlayer_->GetHPComponent(),
        .playerInput = pPlayer_->GetPlayerInput()
    };
    pGameHUD_ = std::make_unique<GameHUD>(hudContext);
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

    // ボスステージでのみボスを生成する
    if (pStage_->GetCurrentStageData().spawnBoss)
    {
        SpawnBoss();
    }
}


void GameScene::Finalize()
{
    pPlayer_->Finalize();
    colliderRepository_.Clear();

#ifdef _DEBUG
    if (pBtEditor_)
    {
        pBtEditor_->Finalize();
    }
#endif

	// エフェクトを削除
	pEmitterManager_->RemoveEmitter("door_open");
	pEmitterManager_->RemoveEmitter("field_background");
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
    // ボスの更新（ボスステージ以外では存在しない）
    if (pBoss_)
    {
        pBoss_->Update();
    }

    pBeatClock_->Update();
    pAttackRepository_->Update();
    
    // 非アクティブの攻撃を削除
    pAttackRepository_->EraseInactiveAttacks();
    // 非アクティブのコライダーを削除
    colliderRepository_.EraseInactiveColliders();

    pCameraDirector_->Update(deltaTime);
    pGameHUD_->Update();

    if (pStage_->IsStageComplete())
    {
        SceneManager::GetInstance()->ChangeScene("title");
        return;
    }
	// ステージ１から２までのクリア条件は、ステージ上の敵を全て倒すこと
    if (pEnemyManager_->IsEmpty(pStage_->GetCurrentIndex()) && !pBoss_)
    {
        // TODO：敵が全部死んだらこいつを呼ぶ
        pStage_->NotifyClear();
    }
    else if (pBoss_ && !pBoss_->IsAlive())
    {
        if (!isBossDeathStarted_)
        {
			// ボスの死亡エフェクトを再生
			pEmitterManager_->CreateTemporaryEmitterFrom("boss_dead", "boss_dead_temp", 2.0f);
			pEmitterManager_->SetEmitterPosition("boss_dead_temp", pBoss_->GetTransform().translate);

            isBossDeathStarted_ = true;
            bossDeathTimer_ = 0.0f;
            bossDeathStartScale_ = pBoss_->GetScale();
        }

        bossDeathTimer_ += deltaTime;
        const float kDeathDuration = 1.5f;
        float t = bossDeathTimer_ / kDeathDuration;
        if (t >= 1.0f)
        {
            t = 1.0f;
            pBoss_->SetScale({ 0.0f, 0.0f, 0.0f });
            pStage_->NotifyClear();
        }
        else
        {
            float easedT = 1.0f - EaseInOutQuint(t);
            pBoss_->SetScale(bossDeathStartScale_ * easedT);
        }
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


#ifdef _DEBUG
    Tako::CollisionManager::GetInstance()->DrawColliders();
#endif // _DEBUG
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
    pEmitterManager_->LoadPreset(Global::ParticleEmitterPresetNames::kPlayerDamaged);
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

void GameScene::SpawnBoss()
{
    isBossDeathStarted_ = false;
    bossDeathTimer_ = 0.0f;

    pBoss_ = std::make_unique<Boss>(pPlayer_.get(), pBeatClock_.get(), pEmitterManager_.get());
    pBoss_->Initialize();

    pGameHUD_->SetBossHPComponent(pBoss_->GetHPComponent());
#ifdef _DEBUG
    // エディタが構築したランタイムツリーを共有し、実行中ノードのハイライトとライブ編集を有効化する
    if (auto root = pBtEditor_->BuildRuntimeTree())
    {
        pBoss_->SetBehaviorTreeRoot(root);
    }

    // ボスのデバッグUIからノードエディタの表示を切り替えられるようにする
    pBoss_->SetNodeEditorToggleCallback([this]()
    {
        pBtEditor_->SetVisible(!pBtEditor_->IsVisible());
    });

    // ボスのデバッグUIとノードエディタの描画をDebugUIManagerへ登録（~BossがUnregisterするため生成のたびに再登録する）
    Tako::DebugUIManager::GetInstance()->RegisterGameObject("Boss", [this]()
    {
        if (!pBoss_)
        {
            return;
        }
        pBoss_->DrawImGui();

        // エディタで編集したツリーをボスへ再適用する
        if (ImGui::Button("Apply Tree To Boss"))
        {
            if (auto root = pBtEditor_->BuildRuntimeTree())
            {
                pBoss_->SetBehaviorTreeRoot(root);
            }
        }

        // ノードエディタの描画と実行中ノードのハイライト
        pBtEditor_->Update();
        if (pBoss_->GetBehaviorTree())
        {
            pBtEditor_->HighlightRunningNode(pBoss_->GetBehaviorTree()->GetCurrentRunningNode());
        }
    });

    
#endif
}

void GameScene::DrawObjects()
{
    pStage_->Draw();
    pPlayer_->Draw();
    pEnemyManager_->Draw(pStage_->GetCurrentIndex());
    if (pBoss_)
    {
        pBoss_->Draw();
    }
}

void GameScene::ApplyPostEffects()
{
    Tako::PostEffectManager::GetInstance()->AddEffectToChain("DepthBasedOutline");
}
