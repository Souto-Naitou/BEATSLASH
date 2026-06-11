#include "HPBarUI.h"

#include <ozSound/audio/JsonUtils/JsonUtils.h>
#include <utility/DeltaTimeManager.h>
#include <TextureManager.h>
#include <math/Easing.h>
#include <utility/ViewportUnits.hpp>
#ifdef _DEBUG
#include <ImGuiManager.h>
#include <DebugUIManager.h>
#endif // _DEBUG
static constexpr const char* kJsonDirectory_ = "resources/json/ui/";

using namespace ozSound;
using namespace Tako;

using json = ozSound::json;


HPBarUI::HPBarUI(const std::string& name) : name_(name)
{
#ifdef _DEBUG
    DebugUIManager::GetInstance()->RegisterGameObject(name_ + "_HPBarUI", std::bind(&HPBarUI::ImGui, this));
#endif // _DEBUG
}

HPBarUI::~HPBarUI()
{
#ifdef _DEBUG
    DebugUIManager::GetInstance()->UnregisterGameObject(name_ + "_HPBarUI");
#endif // _DEBUG
}

void HPBarUI::Initialize()
{
    json j = LoadJson(kJsonDirectory_ + name_ + "HPBarUI.json");

    if (j.empty())
    {
        j = json::object();
    }

    if (j.contains("back"))
    {
        auto jBack = j["back"];

        auto jPos =     jBack.value("position", json::object());
        auto jSize =    jBack.value("size", json::object());
        auto jColor =   jBack.value("color", json::object());

        backgroundData_.position = { jPos.value("x",100.0f), jPos.value("y",100.0f) };
        backgroundData_.size = { jSize.value("x",100.0f), jSize.value("y",100.0f) };
        backgroundData_.color = { jColor.value("x",1.0f), jColor.value("y",1.0f), jColor.value("z",1.0f), jColor.value("w",1.0f) };
        backgroundData_.texturePath = jBack.value("texturePath", "EngineResources/Texture/white.dds");
    }
    if (j.contains("bar"))
    {
        auto jBar = j["bar"];

        auto jPos =     jBar.value("position", json::object());
        auto jSize =    jBar.value("size", json::object());
        auto jColor =   jBar.value("color", json::object());

        barData_.position = { jPos.value("x",100.0f), jPos.value("y",100.0f) };
        barData_.size = { jSize.value("x",100.0f), jSize.value("y",100.0f) };
        barData_.color = { jColor.value("x",1.0f), jColor.value("y",1.0f), jColor.value("z",1.0f), jColor.value("w",1.0f) };
        barData_.texturePath = jBar.value("texturePath", "EngineResources/Texture/white.dds");
    }

    auto jfColor = j.value("frontColor", json::object());
    auto jbColor = j.value("backColor", json::object());

    frontColor_ = { jfColor.value("x", 0.0f), jfColor.value("y", 1.0f), jfColor.value("z", 0.0f), jfColor.value("w", 1.0f) };
    backColor_ = { jbColor.value("x", 1.0f), jbColor.value("y", 0.0f), jbColor.value("z", 0.0f), jbColor.value("w", 1.0f) };

    pBackground_ = std::make_unique<Tako::Sprite>();
    pBackground_->Initialize(backgroundData_.texturePath);
    pBar_ = std::make_unique<Tako::Sprite>();
    pBar_->Initialize(barData_.texturePath);
    pAnimBar_ = std::make_unique<Tako::Sprite>();
    pAnimBar_->Initialize(barData_.texturePath);

    InitSprite(pBackground_.get(), backgroundData_);
    InitSprite(pBar_.get(), barData_);
    InitSprite(pAnimBar_.get(), barData_);

    pAnimBar_->SetColor(backColor_);
    pBar_->SetColor(frontColor_);
}

void HPBarUI::Update(float hpRatio)
{
    if (targetHpRatio_ != hpRatio)
    {
        targetHpRatio_ = hpRatio;
        elapsedAnimTime_ = 0.0f; // アニメーション開始
        pBar_->SetSize(ApplyViewportUnit({ barData_.size.x * targetHpRatio_, barData_.size.y })); // 即座にバーのサイズを更新
    }
    const float deltaTime = DeltaTimeManager::GetInstance()->GetDeltaTime(DeltaTimeChannelReserved::Game);
    elapsedAnimTime_ += deltaTime;
    float progress = (std::min)(elapsedAnimTime_ / barAnimDuration_, 1.0f);
    // ここでイージング
    float easedProgress = Math::Easing::EaseInCubic(progress);
    currentHpRatio_ = currentHpRatio_ * (1.0f - easedProgress) + targetHpRatio_ * easedProgress;

    float newSizeX = barData_.size.x * currentHpRatio_;
    Vector2 newSize = { newSizeX, barData_.size.y };

    pBackground_->SetSize(ApplyViewportUnit(backgroundData_.size));
    pBar_->SetSize(ApplyViewportUnit({ barData_.size.x * targetHpRatio_, barData_.size.y }));
    pAnimBar_->SetSize(ApplyViewportUnit(newSize));

    pBackground_->SetPos(ApplyViewportUnit(backgroundData_.position));
    pBar_->SetPos(ApplyViewportUnit(barData_.position));
    pAnimBar_->SetPos(ApplyViewportUnit(barData_.position));

    pBackground_->SetSize(ApplyViewportUnit(backgroundData_.size));
    pBar_->SetSize(ApplyViewportUnit({ barData_.size.x * targetHpRatio_, barData_.size.y }));
    pAnimBar_->SetSize(ApplyViewportUnit(newSize));

    pBackground_->SetPos(ApplyViewportUnit(backgroundData_.position));
    pBar_->SetPos(ApplyViewportUnit(barData_.position));
    pAnimBar_->SetPos(ApplyViewportUnit(barData_.position));

    pBackground_->Update();
    pBar_->Update();
    pAnimBar_->Update();
}

void HPBarUI::Draw()
{
    pBackground_->Draw();
    pAnimBar_->Draw();
    pBar_->Draw();
}

void HPBarUI::ImGui()
{
#ifdef _DEBUG
    if (ImGuiForSpriteData("Background", backgroundData_))
    {
        InitSprite(pBackground_.get(), backgroundData_);
    }
    if (ImGuiForSpriteData("Bar", barData_))
    {
        InitSprite(pBar_.get(), barData_);
        InitSprite(pAnimBar_.get(), barData_);
    }
    ImGui::SeparatorText("Animation Parameters");

    if (ImGui::ColorEdit4("Front Color", &frontColor_.x))
        pBar_->SetColor(frontColor_);
    if (ImGui::ColorEdit4("Back Color", &backColor_.x))
        pAnimBar_->SetColor(backColor_);


    ImGui::DragFloat("Bar Animation Duration", &barAnimDuration_, 0.1f, 0.1f, 10.0f);
    ImGui::Text("Current HP Ratio: %.2f", currentHpRatio_);
    ImGui::Text("Target HP Ratio: %.2f", targetHpRatio_);
    ImGui::Text("Elapsed Animation Time: %.2f", elapsedAnimTime_);

    if (ImGui::Button("Save"))
    {
        json j = json::object();
        j["back"] = {
            {"position", {{"x", backgroundData_.position.x}, {"y", backgroundData_.position.y}}},
            {"size", {{"x", backgroundData_.size.x}, {"y", backgroundData_.size.y}}},
            {"color", {{"x", backgroundData_.color.x}, {"y", backgroundData_.color.y}, {"z", backgroundData_.color.z}, {"w", backgroundData_.color.w}}},
            {"texturePath", backgroundData_.texturePath}
        };
        j["bar"] = {
            {"position", {{"x", barData_.position.x}, {"y", barData_.position.y}}},
            {"size", {{"x", barData_.size.x}, {"y", barData_.size.y}}},
            {"color", {{"x", barData_.color.x}, {"y", barData_.color.y}, {"z", barData_.color.z}, {"w", barData_.color.w}}},
            {"texturePath", barData_.texturePath}
        };
        j["animation"] = {
            {"barAnimDuration", barAnimDuration_}
        };

        j["frontColor"] = {
            {"x", frontColor_.x},
            {"y", frontColor_.y},
            {"z", frontColor_.z},
            {"w", frontColor_.w}
        };
        j["backColor"] = {
            {"x", backColor_.x},
            {"y", backColor_.y},
            {"z", backColor_.z},
            {"w", backColor_.w}
        };

        ozSound::SaveJson(kJsonDirectory_ + name_ + "HPBarUI.json", j);
    }

#endif // _DEBUG
}

void HPBarUI::InitSprite(Tako::Sprite* pSprite, const SpriteData& data)
{
    pSprite->SetPos(ApplyViewportUnit(data.position));
    pSprite->SetSize(ApplyViewportUnit(data.size));
    pSprite->SetColor(data.color);
    pSprite->SetAnchorPoint({ 0.0f,0.5f });
}

bool HPBarUI::ImGuiForSpriteData(const std::string& label, SpriteData& data)
{
    bool changed = false;
#ifdef _DEBUG
    ImGui::SeparatorText(label.c_str());

    ImGui::PushID(label.c_str());
    changed |= ImGui::DragFloat2("Position", &data.position.x, 1.0f);
    changed |=ImGui::DragFloat2("Size", &data.size.x, 1.0f);
    changed |=ImGui::ColorEdit4("Color", &data.color.x);
    char buff[256];
    strcpy_s(buff, data.texturePath.c_str());
    if (ImGui::InputText("Texture Path", buff, sizeof(buff)))
    {
        data.texturePath = buff;
    }

    if (ImGui::SmallButton("Apply Texture Path"))
    {
        data.texturePath = buff;
        TextureManager::GetInstance()->LoadTexture(data.texturePath);
        if (label == "Background")
        {
            pBackground_->SetTexture(data.texturePath);
        }
        else if (label == "Bar")
        {
            pBar_->SetTexture(data.texturePath);
            pAnimBar_->SetTexture(data.texturePath);
        }
    }
    ImGui::PopID();
#endif // _DEBUG
    return changed;
}

Tako::Vector2 HPBarUI::ApplyViewportUnit(const Tako::Vector2& vec) const
{
    return Tako::Vector2(
        Math::Viewport::Unit::vw(vec.x),
        Math::Viewport::Unit::vh(vec.y)
    );
}