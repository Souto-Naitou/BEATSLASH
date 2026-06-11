#pragma once
#include <Sprite.h>
#include <memory>
#include <input/PlayerAction.h>
#include <debug/GameParameter.h>

#include <presentation/animation/AnimationTimeline.hpp>

/// 操作キーを画面に表示する UI
class ControlKeyUI
{
public:
    void Initialize(bool isGamepadMode);
    void Update(bool isGamepadMode);
    void Draw();

private:

    struct SpriteGroup
    {
        std::unique_ptr<Tako::Sprite> pSprite;
        std::unique_ptr<Tako::Sprite> pbackSprite;
        std::unique_ptr<Tako::Sprite> pbackSprite2;
        Tako::Vector2 pos;
        AnimationTimeline<float> scaleAnim;
    };

    void UpdateSprite(SpriteGroup* psg, const Tako::Vector2& pos, const Tako::Vector2& baseSize);
    void DrawSprite(const SpriteGroup* psg) const;
    void SetUpSprite(SpriteGroup* psg, PlayerAction action, bool isGamepad);
    void InitSpriteGroup(SpriteGroup* psg, PlayerAction action, bool isGamepad);
    void InitTextSprite();
private:

    EnableDebug("ControlKeyUI");

    GameParameter(Tako::Vector2, attackPos_,    Tako::Vector2(85.0f, 70.0f));
    GameParameter(Tako::Vector2, overdrivePos_, Tako::Vector2(85.0f, 80.0f));
    GameParameter(Tako::Vector2, upTempoPos_,   Tako::Vector2(85.0f, 90.0f));
    GameParameter(Tako::Vector2, keySize_,      Tako::Vector2(4.5f, 8.0f));

    std::unique_ptr<SpriteGroup> pAttackSprite_ = nullptr;
    std::unique_ptr<SpriteGroup> pOverdriveSprite_ = nullptr;
    std::unique_ptr<SpriteGroup> pUpTempoSprite_ = nullptr;

    GameParameter(Tako::Vector2, textOffset_, Tako::Vector2(2.0f, 0.0f));
    std::unique_ptr<Tako::Sprite> pAttackTextSprite_ = nullptr;
    std::unique_ptr<Tako::Sprite> pOverdriveTextSprite_ = nullptr;
    std::unique_ptr<Tako::Sprite> pUpTempoTextSprite_ = nullptr;

    bool prevIsGamepad_ = false;
};
