#include "ControlKeyUI.h"
#include <input/KeyConfig.h>

#include <utility/ViewportUnits.hpp>

void ControlKeyUI::Initialize(bool isGamepadMode)
{
    pAttackSprite_ = std::make_unique<SpriteGroup>();
    pOverdriveSprite_ = std::make_unique<SpriteGroup>();
    pUpTempoSprite_ = std::make_unique<SpriteGroup>();

    InitSpriteGroup(pAttackSprite_.get(), PlayerAction::Attack, isGamepadMode);
    InitSpriteGroup(pOverdriveSprite_.get(), PlayerAction::Overdrive, isGamepadMode);
    InitSpriteGroup(pUpTempoSprite_.get(), PlayerAction::UpTempo, isGamepadMode);

    InitTextSprite();

    prevIsGamepad_ = isGamepadMode;
}

void ControlKeyUI::Update(bool isGamepadMode)
{
    // 入力モードが切り替わったらテクスチャを差し替える
    if (isGamepadMode != prevIsGamepad_)
    {
        SetUpSprite(pAttackSprite_.get(), PlayerAction::Attack, isGamepadMode);
        SetUpSprite(pOverdriveSprite_.get(), PlayerAction::Overdrive, isGamepadMode);
        SetUpSprite(pUpTempoSprite_.get(), PlayerAction::UpTempo, isGamepadMode);
        prevIsGamepad_ = isGamepadMode;
    }

    // ヘルパー ：vpに変換した値を返す
    auto toVP = [](const Tako::Vector2& v) -> Tako::Vector2
        {
            return {
             Math::Viewport::Unit::vw(v.x),
             Math::Viewport::Unit::vh(v.y)
            };
        };

    Tako::Vector2 attackPos    = toVP(attackPos_);
    Tako::Vector2 overdrivePos = toVP(overdrivePos_);
    Tako::Vector2 upTempoPos   = toVP(upTempoPos_);
    Tako::Vector2 keySize      = toVP(keySize_);

    auto checkAndAnimate = [&](SpriteGroup* psg, PlayerAction action)
    {
        if (KeyConfig::IsActionTriggered(KeyConfig::GetPad(action), KeyConfig::GetKeyboard(action), isGamepadMode))
            psg->scaleAnim.Start(1.0f);
    };
    checkAndAnimate(pAttackSprite_.get(),    PlayerAction::Attack);
    checkAndAnimate(pOverdriveSprite_.get(), PlayerAction::Overdrive);
    checkAndAnimate(pUpTempoSprite_.get(),   PlayerAction::UpTempo);

    UpdateSprite(pAttackSprite_.get(),    attackPos,    keySize);
    UpdateSprite(pOverdriveSprite_.get(), overdrivePos, keySize);
    UpdateSprite(pUpTempoSprite_.get(),   upTempoPos,   keySize);

    pAttackTextSprite_->SetPos(toVP(attackPos_.Get() + textOffset_));
    pOverdriveTextSprite_->SetPos(toVP(overdrivePos_.Get() + textOffset_));
    pUpTempoTextSprite_->SetPos(toVP(upTempoPos_.Get() + textOffset_));

    pAttackTextSprite_->Update();
    pOverdriveTextSprite_->Update();
    pUpTempoTextSprite_->Update();
}

void ControlKeyUI::Draw()
{
    DrawSprite(pAttackSprite_.get());
    DrawSprite(pOverdriveSprite_.get());
    DrawSprite(pUpTempoSprite_.get());

    pAttackTextSprite_->Draw();
    pOverdriveTextSprite_->Draw();
    pUpTempoTextSprite_->Draw();
}


void ControlKeyUI::UpdateSprite(SpriteGroup* psg, const Tako::Vector2& pos, const Tako::Vector2& baseSize)
{
    float scale = psg->scaleAnim.IsPlaying() ? psg->scaleAnim.Update() : 1.0f;
    Tako::Vector2 scaledSize = { baseSize.x * scale, baseSize.y * scale };

    psg->pSprite->SetPos(pos);
    psg->pbackSprite->SetPos(pos);
    psg->pbackSprite2->SetPos(pos);

    psg->pSprite->SetSize(scaledSize);
    psg->pbackSprite->SetSize(scaledSize);
    psg->pbackSprite2->SetSize(scaledSize);

    psg->pSprite->Update();
    psg->pbackSprite->Update();
    psg->pbackSprite2->Update();
}

void ControlKeyUI::DrawSprite(const SpriteGroup* psg) const
{
    psg->pbackSprite2->Draw();
    psg->pbackSprite->Draw();
    psg->pSprite->Draw();
}

void ControlKeyUI::SetUpSprite(SpriteGroup* psg, PlayerAction action, bool isGamepad)
{
    const auto& kbConf = KeyConfig::GetKeyboard(action);
    const auto& pdConf = KeyConfig::GetPad(action);

    psg->pSprite->SetTexture(isGamepad ? pdConf.imagePath : kbConf.imagePath);

    std::string backTexturePath = isGamepad ? pdConf.backgroundImagePath : kbConf.backgroundImagePath;
    psg->pbackSprite->SetTexture(backTexturePath);
    psg->pbackSprite2->SetTexture(backTexturePath);

    // ゲームパッドの場合、左右反転の設定も適用
    bool filped = isGamepad ? pdConf.flipU : false;
    psg->pbackSprite->SetIsFlipX(filped);
    psg->pbackSprite2->SetIsFlipX(filped);
}


void ControlKeyUI::InitSpriteGroup(SpriteGroup* psg, PlayerAction action, bool isGamepad)
{
    psg->pSprite = std::make_unique<Tako::Sprite>();
    psg->pbackSprite = std::make_unique<Tako::Sprite>();
    psg->pbackSprite2 = std::make_unique<Tako::Sprite>();

    psg->pSprite->Initialize("");
    psg->pbackSprite->Initialize("");
    psg->pbackSprite2->Initialize("");

    psg->pbackSprite->SetColor({ 0.3f, 0.3f, 0.3f, 1.0f });
    psg->pbackSprite2->SetColor({ 0.3f, 0.3f, 0.3f, 0.5f });

    psg->pSprite->SetAnchorPoint({ 0.5f, 0.5f });
    psg->pbackSprite->SetAnchorPoint({ 0.5f, 0.5f });
    psg->pbackSprite2->SetAnchorPoint({ 0.5f, 0.5f });

    psg->scaleAnim.AddTween(0.0f, 0.2f, 1.0f, 1.3f);
    psg->scaleAnim.AddTween(0.2f, 0.3f, 1.3f, 1.0f);

    SetUpSprite(psg, action, isGamepad);
}

void ControlKeyUI::InitTextSprite()
{
    pAttackTextSprite_ = std::make_unique<Tako::Sprite>();
    pOverdriveTextSprite_ = std::make_unique<Tako::Sprite>();
    pUpTempoTextSprite_ = std::make_unique<Tako::Sprite>();

    pAttackTextSprite_->Initialize("text_attack.png");
    pOverdriveTextSprite_->Initialize("text_overdrive.png");
    pUpTempoTextSprite_->Initialize("text_upTempo.png");

    pAttackTextSprite_->SetAnchorPoint({ 0.0f, 0.5f });
    pOverdriveTextSprite_->SetAnchorPoint({ 0.0f, 0.5f });
    pUpTempoTextSprite_->SetAnchorPoint({ 0.0f, 0.5f });
}
