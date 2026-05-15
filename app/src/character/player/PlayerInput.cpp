#include "PlayerInput.h"



void PlayerInput::Initialize()
{
    pInput_ = Tako::Input::GetInstance();
}

void PlayerInput::Update()
{
    // 前回の入力データ
    PlayerInput::Data preData = data_;
    data_ = {};
    
    /// [ ゲームパッドモードの切り替え ]


    /// [ プレイヤーの入力用に変換 ]
    // 移動
    auto dirX = pInput_->PushKey(DIK_D) - pInput_->PushKey(DIK_A);
    auto dirZ = pInput_->PushKey(DIK_W) - pInput_->PushKey(DIK_S);
    data_.move = Tako::Vector3(static_cast<float>(dirX), 0.0f, static_cast<float>(dirZ));
    data_.move = data_.move.Normalize();
}
