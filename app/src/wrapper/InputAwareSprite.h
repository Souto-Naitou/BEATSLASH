#pragma once
#include <Sprite.h>
#include <system/EventSubscription.h>
#include <optional>
#include <system/EventListener.h>
#include <Vector2.h>

/// <summary>
/// 入力デバイスに応じて表示を切り替えるスプライトクラス。
/// </summary>
class InputAwareSprite
{
public:
    struct Entry
    {
        Tako::Sprite* pSprite_ = nullptr;
        uint32_t handleKeyboard_ = {};
        std::optional<Tako::Vector2> sizeKeyboard_ = std::nullopt;
        std::optional<Tako::Vector2> positionKeyboard_ = std::nullopt;
        uint32_t handleGamepad_ = {};
        std::optional<Tako::Vector2> sizeGamepad_ = std::nullopt;
        std::optional<Tako::Vector2> positionGamepad_ = std::nullopt;
    };
    void Initialize();

    void AddEntry(const Entry& entry)
    {
        entries_.push_back(entry);
    }

    /// <summary>
    /// 現在の入力モードをテクスチャに適用します。
    /// </summary>
    void ApplyCurrentMode();

private:
    void RegisterSubscriptions();
    void ApplyToGamepad();
    void ApplyToKeyboard();

    std::vector<std::optional<EventSubscription>> subscriptions_;
    std::vector<Entry> entries_;

    // イベントリスナー
    EventListener* pEventListener_ = EventListener::GetInstance();
};