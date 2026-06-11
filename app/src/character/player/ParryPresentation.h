#pragma once
#include <EmitterManager.h>
#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include <Vector3.h>
#include <system/EventSubscription.h>
#include <event/PlayerParryEvent.h>

class ParryPresentation
{
public:
    ParryPresentation(Tako::EmitterManager& emitterManager, const Tako::Vector3& playerPosition);
    void Play(const Tako::Vector3& position);
    void Update();
    void OnParryReject(const Event::PlayerParry::Rejected&);
    void OnparryTrue(const Event::PlayerParry::True&);

private:
    // 生存中の一時エミッター。生存中はプレイヤー位置に追従させる
    struct ActiveEffect
    {
        std::string name;
        float remainingTime;
        Tako::Vector3 offset; // 生成時のプレイヤー位置との差分（追従中も維持する）
    };

    static constexpr float kEffectLifeTime_ = 0.3f; // 一時エミッターの寿命

    uint32_t uniqueIndexSuccess_ = 0; // エミッターの一意なインデックス生成用
    uint32_t uniqueIndexFail_ = 0; // エミッターの一意なインデックス生成用
    uint32_t uniqueIndexTrue_ = 0; // エミッターの一意なインデックス生成用
    Tako::EmitterManager& emitterManager_;
    const Tako::Vector3& playerPosition_; // プレイヤー位置（追従用）
    std::vector<ActiveEffect> activeEffects_;
    std::optional<EventSubscription> subsRejectParry_;
    std::optional<EventSubscription> subsTrueParry_;
};
