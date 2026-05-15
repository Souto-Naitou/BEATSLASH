#pragma once
#include <d3d12.h>

#include <array>
#include <span>
#include <vector>
#include <Sprite.h>
#include <font/FontLayout.h>
#include <Vector4.h>
#include <memory>

class NumericView
{
public:
    using TextureHandleType = uint32_t;
    
    void Initialize(
        std::span<TextureHandleType> textureHandles, 
        const std::string& name
    );
    void Finalize();

    void Update();
    void Draw1F();
    void ImGui();

    float GetFontSize() const { return fontSizeY_; }
    void SetFontSize(float sizeY);
    void SetNumber(uint32_t number) { currentNumber_ = number; }
    void SetColor(const Tako::Vector4& color);
    FontLayout::Properties& GetFontLayoutProperties() { return fontLayout_.GetProperties(); }
    Tako::Vector4 GetColor() const { return color_; }

private:
    void AddSprite();
    uint32_t GetDigitCount() const;
    static constexpr uint32_t kRadix_ = 10u;

    std::array<TextureHandleType, kRadix_> numberTextureHandles_ = {};

    // デバッグ用の名前
    std::string debugName_;
    // フォントレイアウト
    FontLayout fontLayout_;

    std::vector<std::unique_ptr<Tako::Sprite>> numberSprites_;
    std::vector<GlyphInfo> glyphInfos_;

    float fontSizeY_ = 32.0f;

    Tako::Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

    uint32_t currentNumber_ = 0;
};