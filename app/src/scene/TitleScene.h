#pragma once

#include <BaseScene.h>
#include <memory>
#include <Sprite.h>

class TitleScene : public Tako::BaseScene
{
public:



    void Initialize() override;


    void Finalize() override;


    void Update() override;


    void Draw() override;


    void DrawWithoutEffect() override;


    void DrawImGui() override;

private:
    std::unique_ptr<Tako::Sprite> pSpriteTitle_;
    std::unique_ptr<Tako::Sprite> pSpriteStartPrompt_;
};