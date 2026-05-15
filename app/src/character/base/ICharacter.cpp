#include "ICharacter.h"

void ICharacter::Initialize()
{

    // コライダーの初期化
    collider_ = std::make_unique<CharacterCollider>();
    collider_->SetSize({ 1.0f, 1.0f, 1.0f });       // デフォルトのサイズを設定
    collider_->SetOwner(this);// コライダーの所有者をこのキャラクターに設定
    collider_->SetTypeID(static_cast<uint32_t>(colliderID)); // コライダーの型IDを設定
    collider_->SetTransform(&transform_);			// コライダーのトランスフォームをキャラクターのトランスフォームに紐づける
}

void ICharacter::Update()
{
    // トランスフォームの更新
    model_->SetTransform(transform_);
    model_->Update();
}

void ICharacter::Draw()
{
}
