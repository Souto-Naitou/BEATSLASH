#pragma once

#include <debug/GameParameter.h>
#include "PlayerInput.h"
#include "PlayerMovement.h"
#include <character/ICharacter.h>
#include <Object3d.h>
#include <memory>

class Player : public ICharacter
{
public:
    void Initialize() override;
    void Finalize();
    void Update() override;
    void Draw() override;
    void ImGui();

private:
    void InitializeComponents();

    /// パラメータ
    GameParameter float kFrictionPower_     = 0.8f;      // 摩擦係数
    GameParameter float kMovePower_         = 180.0f;    // 移動力

    /// インスタンス
    std::unique_ptr<PlayerInput>        pInput_;        // プレイヤー入力管理クラス
    std::unique_ptr<PlayerMovement>     pMovement_;     // プレイヤー移動処理クラス
    std::unique_ptr<Tako::Object3d>     pModel_;        // キャラクターの3Dモデル
    Tako::Transform                     transform_;     // キャラクターのトランスフォーム

};