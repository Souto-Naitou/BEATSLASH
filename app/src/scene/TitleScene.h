#pragma once

#include <BaseScene.h>
#include <memory>
#include <Sprite.h>
#include <stage/StageSequence.h>
#include <entity/camera/RailCamera.h>
#include <presentation/animation/AnimationTimeline.hpp>
#include <Vector3.h>
#include "debug/GameParameter.h"
#include <ozSound/audio/SoundEngine.h>

class TitleScene : public Tako::BaseScene
{
public:



    void Initialize() override;


    void Finalize() override;


    void Update() override;


    void Draw() override;


    void DrawWithoutEffect() override;


    void DrawImGui() override;

private:
    std::unique_ptr<Tako::Sprite>   pSpriteTitle_;
    std::unique_ptr<Tako::Sprite>   pSpriteStartPrompt_;
    std::unique_ptr<StageSequence>  pStage_;                // !< ステージクラスのインスタンス
    std::unique_ptr<RailCamera>     pRailCamera_;           // !< レールカメラのインスタンス
    AnimationTimeline<Tako::Vector3> railCameraTimeline_;   // !< レールカメラの軌道データ
	// BGMのサウンドハンドル
    ozSound::SoundHandle bgmHandle_;

    EnableDebug("TitleScene");
    GameParameter(Tako::Vector3, kCenter_, Tako::Vector3(-20.0f, 30.0f, 0.0f));
};