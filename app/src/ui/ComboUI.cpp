#include "ComboUI.h"
#include <TextureManager.h>

// TODO:WinAppのインクルードは良くないので引数などで持ってくる
#include <WinApp.h>

void ComboUI::Initialize()
{
    // 数字テクスチャの読み込み
    LoadNumberTextures();

    comboValue_.Initialize(numberTextureHandles_, "ComboValue");
    comboValue_.SetFontSize(comboFontSize);
    comboValue_.SetColor(comboFontColor);
    // フォントレイアウトの初期設定 
    auto& fontLayoutProps = comboValue_.GetFontLayoutProperties();
    fontLayoutProps.leftTop = *comboFontLayoutLeftTop.GetPtr();
    //中央を基準点にする
    fontLayoutProps.anchorPoint = { 0.5f, 0.5f };

    RegisterCallvacks();
}

void ComboUI::Update(uint32_t combo)
{
    comboValue_.SetNumber(combo);
    comboValue_.Update();
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
        comboValue_.SetFontSize(newValue);
    });
    comboFontColor.SetOnChange([this](const Tako::Vector4& newColor)
    {
        comboValue_.SetColor(newColor);
    });
    comboFontLayoutLeftTop.SetOnChange([this](const Tako::Vector2& newLeftTop)
    {
        auto& fontLayoutProps = comboValue_.GetFontLayoutProperties();
        fontLayoutProps.leftTop = newLeftTop;
    });
#endif // _DEBUG
}
