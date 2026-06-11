#pragma once
#include <memory>

#include <Sprite.h>

#include <debug/GameParameter.h>

class PlayerHPBarUI
{
public:

    PlayerHPBarUI();
    ~PlayerHPBarUI();

    void Initialize();
    void Update(float hpRatio);
    void Draw();

private:

    struct SpriteData
    {
        Tako::Vector2 position;
        Tako::Vector2 size;
        Tako::Vector4 color;
        std::string texturePath;
    };

    void ImGui();

    void InitSprite(Tako::Sprite* pSprite, const SpriteData& data);

    bool ImGuiForSpriteData(const std::string& label, SpriteData& data);

    Tako::Vector2 ApplyViewportUnit(const Tako::Vector2& vec) const;
private:

    SpriteData backgroundData_; // 背景枠のスプライトデータ
    SpriteData barData_;        // HPバーのスプライトデータ

    // 背景枠
    std::unique_ptr<Tako::Sprite> pBackground_;
    // HPバー 現在のHPを表す
    std::unique_ptr<Tako::Sprite> pBar_;
    // バーアニメーションのためのスプライト
    std::unique_ptr<Tako::Sprite> pAnimBar_;

    EnableDebug("PlayerHPBarUI");

    float barAnimDuration_ = 1.0f; // バーアニメーションの継続時間
    float elapsedAnimTime_ = 0.0f; // バーアニメーションの経過時間
    float targetHpRatio_ = 1.0f; // バーアニメーションの目標HP比率
    float currentHpRatio_ = 1.0f; // バーアニメーションの現在HP比率
};