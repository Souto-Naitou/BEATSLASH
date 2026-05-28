#pragma once

#include <font/NumericView.h>

#include <debug/GameParameter.h>

class ComboUI
{

public:

    void Initialize();
    void Update(uint32_t combo);
    void Draw();



private:

    void LoadNumberTextures();

    void RegisterCallvacks();
private:

    EnableDebug("ComboUI");

    NumericView comboValue_;

    std::vector<NumericView::TextureHandleType> numberTextureHandles_;

    GameParameter(float, comboFontSize, 100.0f);
    GameParameter(Tako::Vector4, comboFontColor, Tako::Vector4(0.42f, 0.5f, 0.8f, 1.0f));
    GameParameter(Tako::Vector2, comboFontLayoutLeftTop, Tako::Vector2(1500.0f, 540.0f));


};
