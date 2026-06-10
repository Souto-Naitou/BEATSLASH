#include "RhythmHint.h"

#include <ozSound/audio/JsonUtils/JsonUtils.h>
#include <TextureManager.h>
#ifdef _DEBUG
#include <ImGuiManager.h>
#include <DebugUIManager.h>
#include <Input.h>
#endif

static constexpr const char* kJsonPath_ = "resources/json/ui/RhythmHintUI.json";
static constexpr size_t kMaxNotes_ = 10;


using namespace ozSound;
using namespace Tako;

void RhythmHintUI::Initialize()
{
#ifdef _DEBUG
    DebugUIManager::GetInstance()->RegisterGameObject("RhythmHintUI", [this]()
                                                      {
                                                          ImGui();
                                                      });
#endif

    json j = ozSound::LoadJson(kJsonPath_);

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

        backSpriteData_.position = { jPos.value("x",100.0f), jPos.value("y",100.0f) };
        backSpriteData_.size = { jSize.value("x",100.0f), jSize.value("y",100.0f) };
        backSpriteData_.color = { jColor.value("x",1.0f), jColor.value("y",1.0f), jColor.value("z",1.0f), jColor.value("w",1.0f) };
        backSpriteData_.texturePath = jBack.value("texturePath", "EngineResources/Texture/white.dds");

    }
    if (j.contains("center"))
    {
        auto jCenter = j["center"];

        auto jPos =     jCenter.value("position", json::object());
        auto jSize =    jCenter.value("size", json::object());
        auto jColor =   jCenter.value("color", json::object());

        centerSpriteData_.position = { jPos.value("x",100.0f), jPos.value("y",100.0f) };
        centerSpriteData_.size = { jSize.value("x",100.0f), jSize.value("y",100.0f) };
        centerSpriteData_.color = { jColor.value("x",1.0f), jColor.value("y",1.0f), jColor.value("z",1.0f), jColor.value("w",1.0f) };
        centerSpriteData_.texturePath = jCenter.value("texturePath", "EngineResources/Texture/white.dds");
    }
    if (j.contains("note"))
    {
        auto jNote = j["note"];

        auto jPos =     jNote.value("position", json::object());
        auto jSize =    jNote.value("size", json::object());
        auto jColor =   jNote.value("color", json::object());
        auto jTexturePath = jNote.value("texturePath", "EngineResources/Texture/white.dds");

        noteSpriteData_.position = { jPos.value("x",100.0f), jPos.value("y",100.0f) };
        noteSpriteData_.size = { jSize.value("x",100.0f), jSize.value("y",100.0f) };
        noteSpriteData_.color = { jColor.value("x",1.0f), jColor.value("y",1.0f), jColor.value("z",1.0f), jColor.value("w",1.0f) };
        noteSpriteData_.texturePath = jNote.value("texturePath", "EngineResources/Texture/white.dds");
    }

    pBackSprite_ = std::make_unique<Tako::Sprite>();
    pCenterSprite_ = std::make_unique<Tako::Sprite>();

    pBackSprite_->Initialize(backSpriteData_.texturePath);
    pCenterSprite_->Initialize(centerSpriteData_.texturePath);
    InitSprite(pBackSprite_.get(), backSpriteData_);
    InitSprite(pCenterSprite_.get(), centerSpriteData_);


    for (size_t i = 0; i < kMaxNotes_; ++i)
    {
        auto note = std::make_unique<Tako::Sprite>();
        note->Initialize(noteSpriteData_.texturePath);
        InitSprite(note.get(), noteSpriteData_);
        pNotesSprites_.push_back(std::move(note));
    }
}

void RhythmHintUI::Update(float progress)
{
    pBackSprite_->Update();
    pCenterSprite_->Update();

    Vector2 centerPos = centerSpriteData_.position;
    auto it = pNotesSprites_.begin();
    size_t pair = 0;
    while (it != pNotesSprites_.end())
    {
        auto& note = *it++;
        if (it == pNotesSprites_.end()) break;
        auto& note2 = *it++;

        float p = progress - static_cast<float>(pair);
        Vector2 offset = Vector2::Lerp({ 0.0f,0.0f }, { pixelsPerBeat_,0.0f }, p);

        if (p > 0.0f)
        {
            offset.x = 10000.0f; // 画面外へ
        }
        float backHalfSize = pBackSprite_->GetSize().x / 2.0f;
        if (std::abs(offset.x) >= backHalfSize)
        {
            // 枠外のときは非表示
            note->SetAlpha(0.0f);
            note2->SetAlpha(0.0f);
        }
        else
        {// 枠内のときは表示
            note->SetAlpha(1.0f);
            note2->SetAlpha(1.0f);
        }

        note->SetPos(centerPos + offset);
        note2->SetPos(centerPos - offset);

        note->Update();
        note2->Update();

        ++pair;
    }
    if (progress > 0.0f && prevProgress_ <= 0.0f)
    {
        pNotesSprites_.splice(pNotesSprites_.end(),
                              pNotesSprites_,
                              pNotesSprites_.begin(),
                              std::next(pNotesSprites_.begin(), 2));
    }

#ifdef _DEBUG
    if (Tako::Input::GetInstance()->TriggerKey(DIK_T))
        trigerProgress_ = progress;
#endif


    prevProgress_ = progress;
}

void RhythmHintUI::Draw()
{
    pBackSprite_->Draw();
    pCenterSprite_->Draw();

    for (auto& note : pNotesSprites_)
    {
        note->Draw();
    }
}

void RhythmHintUI::ImGui()
{
#ifdef _DEBUG
    ImGui::Begin("RhythmHintUI");

    ImGui::Text("Triger Progress: %.2f", trigerProgress_);


    if (ImGuiForSpriteData("Back Sprite", backSpriteData_))
    {
        InitSprite(pBackSprite_.get(), backSpriteData_);
    }
    if (ImGuiForSpriteData("Center Sprite", centerSpriteData_))
    {
        InitSprite(pCenterSprite_.get(), centerSpriteData_);
    }
    if (ImGuiForSpriteData("Note Sprite", noteSpriteData_))
    {
        for (auto& note : pNotesSprites_)
        {
            InitSprite(note.get(), noteSpriteData_);
        }
    }

    // ラムダ式
    auto saveJson = [this](json& j, const std::string& label, const SpriteData& data)
        {
            j[label] = {
                {"position", {{"x", data.position.x}, {"y", data.position.y}}},
                {"size", {{"x", data.size.x}, {"y", data.size.y}}},
                {"color", {{"x", data.color.x}, {"y", data.color.y}, {"z", data.color.z}, {"w", data.color.w}}},
                {"texturePath", data.texturePath}
            };
        };

    if (ImGui::Button("Save"))
    {
        json j = json::object();
        saveJson(j, "back", backSpriteData_);
        saveJson(j, "center", centerSpriteData_);
        saveJson(j, "note", noteSpriteData_);
        ozSound::SaveJson(kJsonPath_, j);
    }
    ImGui::End();

#endif // _DEBUG

}

bool RhythmHintUI::ImGuiForSpriteData(const std::string& label, SpriteData& data)
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
        pBackSprite_->SetTexture(data.texturePath);
    }
    ImGui::PopID();

#endif // _DEBUG
    return changed;
}

void RhythmHintUI::InitSprite(Sprite* pSprite, const SpriteData& data)
{
    pSprite->SetPos(data.position);
    pSprite->SetSize(data.size);
    pSprite->SetColor(data.color);
    pSprite->SetAnchorPoint({ 0.5f,0.5f });
}
