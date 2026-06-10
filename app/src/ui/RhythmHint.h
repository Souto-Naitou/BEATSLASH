#pragma once

#include <Sprite.h> 
#include <memory>
#include <list>
#include <debug/GameParameter.h>

class RhythmHintUI
{
public:

    RhythmHintUI() = default;
    ~RhythmHintUI() = default;

    void Initialize();
    void Update(float progress);
    void Draw();

private:


    struct SpriteData
    {
        Tako::Vector2 position;
        Tako::Vector2 size;
        Tako::Vector4 color;
        std::string texturePath;
    };
    void InitSprite(Tako::Sprite* pSprite, const SpriteData& data);

    void ImGui();

    bool ImGuiForSpriteData(const std::string& label, SpriteData& data);

private:

    EnableDebug("RhythmHintUI");

    GameParameter(float, pixelsPerBeat_, 150.0f);


    SpriteData backSpriteData_;
    SpriteData centerSpriteData_;
    SpriteData noteSpriteData_;

    std::list<std::unique_ptr<Tako::Sprite>> pNotesSprites_;
    std::unique_ptr<Tako::Sprite> pBackSprite_;
    std::unique_ptr<Tako::Sprite> pCenterSprite_;

    float prevProgress_ = 0.0f; // ゼロ通過検出用

#ifdef _DEBUG
    float trigerProgress_ = 0.0f; // ゼロ通過トリガー用
#endif // _DEBUG

};