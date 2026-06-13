#include "ParryHistory.h"
#include <limits>
#include <string>
#include <format>
#include <debug/DebugRegisterer.h>
#include <system/EventListener.h>
#include <event/PlayerParryEvent.h>
a
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

ParryHistory::ParryHistory(const BeatClock& beatClock_) : beatClock_(beatClock_)
{
    DebugRegister("ParryHistory", &ParryHistory::ImGui, this);
    stopWatch_.Start();
}

ParryHistory::~ParryHistory()
{
    DebugUnregister("ParryHistory");
}

void ParryHistory::Record(const Tako::Vector3& position)
{
    if (GetTimeSinceLastParry() < beatClock_.GetSecondsPerBeat() * kNumParryCooldownBeat_)
    {
        /// EVENT: クールダウン中のパリィ拒否通知を発行
        EventListener::GetInstance()->Publish(Event::PlayerParry::Rejected(position));
        return;
    }

    EventListener::GetInstance()->Publish(Event::PlayerParry::True(position));
    parryTimes_.push_back(std::chrono::steady_clock::now());
}

float ParryHistory::GetTimeSinceLastParry() const
{
    float timeSinceLastHit = std::numeric_limits<float>::max(); // 記録されていない場合は最大値を返す

    if (!parryTimes_.empty())
    {
        auto now = std::chrono::steady_clock::now();
        auto lastHitTime = parryTimes_.back();
        timeSinceLastHit = std::chrono::duration<float>(now - lastHitTime).count();
    }

    return timeSinceLastHit;
}

void ParryHistory::ImGui()
{
    #ifdef _DEBUG

    std::string label = std::format("Parry History - {} entries", static_cast<uint32_t>(parryTimes_.size()));

    ImGui::SeparatorText(label.c_str());

    for (auto& time : parryTimes_)
    {
        auto timeSinceHit = std::chrono::duration<float>(std::chrono::steady_clock::now() - time).count();
        ImGui::Text("Time since parry: %.2f seconds", timeSinceHit);
    }

    #endif // _DEBUG
}
