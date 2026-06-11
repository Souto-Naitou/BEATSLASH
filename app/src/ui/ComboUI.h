#pragma once

#include <font/NumericView.h>

#include <debug/GameParameter.h>
#include <presentation/animation/AnimationTimeline.hpp>
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

    uint32_t currentCombo_ = 0;

    AnimationTimeline<float> scaleAnimTl_={};


    GameParameter(float, comboFontSize, 8.0f);
    GameParameter(Tako::Vector4, comboFontColor, Tako::Vector4(0.42f, 0.5f, 0.8f, 1.0f));
    GameParameter(Tako::Vector2, comboFontLayoutLeftTop, Tako::Vector2(80.0f, 40.0f));


};
