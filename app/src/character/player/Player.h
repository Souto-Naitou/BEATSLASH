#pragma once

#include <debug/GameParameter.h>

#include "PlayerInput.h"
#include "PlayerMovement.h"
#include <character/ICharacter.h>
#include <Object3d.h>
#include <memory>
#include <Transform.h>

class Player : public ICharacter
{
public:
    void Initialize() override;
    void Finalize();
    void Update() override;
    void Draw() override;
    void RegisterCallbacks();

private:
    void InitializeComponents();

    EnableDebug("Player");

    /// パラメータ
    GameParameter(float, kFrictionPower_, 0.8f);    // 摩擦係数
    GameParameter(float, kMovePower_, 180.0f);      // 移動力
    GameParameter(float, kJumpPower_, 5.0f);        // ジャンプ力
    GameParameter(float, kMass_, 60.0f);            // 重力

    /// インスタンス
    std::unique_ptr<PlayerInput>        pInput_;            // プレイヤー入力管理クラス
    std::unique_ptr<PlayerMovement>     pMovement_;         // プレイヤー移動処理クラス
    std::unique_ptr<Tako::Object3d>     pModel_;            // キャラクターの3Dモデル
    GameParameterView(Tako::Transform,  transform_, {});    // キャラクターのトランスフォーム

};