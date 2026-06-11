#include "ComboUI.h"
#include <TextureManager.h>

// TODO:WinAppのインクルードは良くないので引数などで持ってくる
#include <utility/ViewportUnits.hpp>


void ComboUI::Initialize()
{
    // 数字テクスチャの読み込み
    LoadNumberTextures();

    comboValue_.Initialize(numberTextureHandles_, "ComboValue");
    comboValue_.SetFontSize(Math::Viewport::Unit::vw(comboFontSize));
    comboValue_.SetColor(comboFontColor);
    // フォントレイアウトの初期設定 
    auto& fontLayoutProps = comboValue_.GetFontLayoutProperties();
    fontLayoutProps.leftTop = Tako::Vector2(Math::Viewport::Unit::vw(comboFontLayoutLeftTop->x), Math::Viewport::Unit::vh(comboFontLayoutLeftTop->y));
    //中央を基準点にする
    fontLayoutProps.anchorPoint = { 0.5f, 0.5f };
    fontLayoutProps.letterSpacing = -0.05f * Math::Viewport::Unit::vw(1.0f); // 文字間を少し詰める
    AnimationTween<float> tween0(0.0f, 0.15f, 1.0f, 1.4f);
    //tween0.SetTransitionFunction(&Math::Easing::EaseInQuad);
    AnimationTween<float> tween1(0.15f, 0.3f, 1.4f, 1.0f);
    //tween1.SetTransitionFunction(&Math::Easing::EaseInQuad);
    scaleAnimTl_.ClearTween();
    scaleAnimTl_.AddTween(tween0);
    scaleAnimTl_.AddTween(tween1);

    RegisterCallvacks();
}

void ComboUI::Update(uint32_t combo)
{
    if (currentCombo_ != combo)
    {
        // アニメーション
        scaleAnimTl_.Start();
    }

    if (scaleAnimTl_.IsPlaying())
    {
        float scale = scaleAnimTl_.Update();
        float newSize = comboFontSize * scale;

        comboValue_.SetFontSize(newSize);
    }
    auto& fontLayoutProps = comboValue_.GetFontLayoutProperties();
    fontLayoutProps.leftTop = Tako::Vector2(Math::Viewport::Unit::vw(comboFontLayoutLeftTop->x), Math::Viewport::Unit::vh(comboFontLayoutLeftTop->y));
    fontLayoutProps.letterSpacing = -3.0f * Math::Viewport::Unit::vw(1.0f); // 文字間を少し詰める

    comboValue_.SetFontSize(Math::Viewport::Unit::vw(comboFontSize));

    comboValue_.SetNumber(combo);
    comboValue_.Update();

    currentCombo_ = combo;
}

void ComboUI::Draw()
{
    comboValue_.Draw();
}

void ComboUI::LoadNumberTextures()
{
    const std::string kNumberTexturePrefix = "numbers/number_";
    const std::string kNumberTextureSuffix = ".dds";

    auto textureManager = Tako::TextureManager::GetInstance();

    for (int32_t i = 0; i < 10; ++i)
    {
        std::string fileName = kNumberTexturePrefix + std::to_string(i) + kNumberTextureSuffix;
        textureManager->LoadTexture(fileName);
        numberTextureHandles_.push_back(textureManager->GetSRVIndex(fileName));
    }
}

void ComboUI::RegisterCallvacks()
{
#ifdef _DEBUG
    // デバッグ用のコールバック登録
    comboFontSize.SetOnChange([this](float newValue)
    {
        comboValue_.SetFontSize(Math::Viewport::Unit::vw(newValue));
    });
    comboFontColor.SetOnChange([this](const Tako::Vector4& newColor)
    {
        comboValue_.SetColor(newColor);
    });
    comboFontLayoutLeftTop.SetOnChange([this](const Tako::Vector2& newLeftTop)
    {
        auto& fontLayoutProps = comboValue_.GetFontLayoutProperties();
        fontLayoutProps.leftTop = Tako::Vector2(Math::Viewport::Unit::vw(newLeftTop.x), Math::Viewport::Unit::vh(newLeftTop.y));
    });
#endif // _DEBUG
}
