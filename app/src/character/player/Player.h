#pragma once

#include "PlayerInput.h"
#include "PlayerMovement.h"
#include <character/base/ICharacter.h>
#include <Object3d.h>
#include <memory>

class Player : public ICharacter
{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    void InitializeComponents();
    std::unique_ptr<PlayerInput> pInput_;           // < プレイヤー入力管理クラス
    std::unique_ptr<PlayerMovement> pMovement_;     // < プレイヤー移動処理クラス

    std::unique_ptr<Tako::Object3d> pModel_;        // < キャラクターの3Dモデル
    Tako::Transform transform_;                     // < キャラクターのトランスフォーム

};