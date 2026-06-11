#include "InputAwareSprite.h"
#include <event/InputCallbackEvent.h>
#include <Input.h>


void InputAwareSprite::Initialize()
{
    this->RegisterSubscriptions();
}

void InputAwareSprite::ApplyCurrentMode()
{
    if (entries_.empty()) return;

    if (Tako::Input::GetInstance()->IsConnect())
    {
        this->ApplyToGamepad();
    }
    else
    {
        this->ApplyToKeyboard();
    }
}

void InputAwareSprite::RegisterSubscriptions()
{
    subscriptions_.emplace_back() = pEventListener_->Subscribe<Events::GamePadConnected>(
        [this](const Events::GamePadConnected& e)
    {
        this->ApplyToGamepad();
    });

    subscriptions_.emplace_back() = pEventListener_->Subscribe<Events::GamePadDisconnected>(
        [this](const Events::GamePadDisconnected& e)
    {
        this->ApplyToKeyboard();
    });
}

void InputAwareSprite::ApplyToGamepad()
{
    for (const auto& entry : entries_)
    {
        entry.pSprite_->SetTextureIndex(entry.handleGamepad_);
        if (entry.sizeGamepad_) entry.pSprite_->SetSize(entry.sizeGamepad_.value());
        if (entry.positionGamepad_) entry.pSprite_->SetPos(entry.positionGamepad_.value());
    }
}

void InputAwareSprite::ApplyToKeyboard()
{
    for (const auto& entry : entries_)
    {
        entry.pSprite_->SetTextureIndex(entry.handleKeyboard_);
        if (entry.sizeKeyboard_) entry.pSprite_->SetSize(entry.sizeKeyboard_.value());
        if (entry.positionKeyboard_) entry.pSprite_->SetPos(entry.positionKeyboard_.value());
    }
}
