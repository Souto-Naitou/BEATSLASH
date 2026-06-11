#pragma once
#include <EmitterManager.h>
#include <string>
#include <optional>
#include <cstdint>
#include <Vector3.h>
#include <system/EventSubscription.h>
#include <event/PlayerParryEvent.h>

class ParryPresentation
{
public:
    ParryPresentation(Tako::EmitterManager& emitterManager);
    void Play(const Tako::Vector3& position);
    void Update();
    void OnParryReject(const Event::PlayerParry::Rejected&);
    void OnparryTrue(const Event::PlayerParry::True&);

private:
    uint32_t uniqueIndexSuccess_ = 0; // エミッターの一意なインデックス生成用
    uint32_t uniqueIndexFail_ = 0; // エミッターの一意なインデックス生成用
    uint32_t uniqueIndexTrue_ = 0; // エミッターの一意なインデックス生成用
    Tako::EmitterManager& emitterManager_;
    std::string uniqueNameParryEffect_;
    std::optional<EventSubscription> subsRejectParry_;
    std::optional<EventSubscription> subsTrueParry_;
};