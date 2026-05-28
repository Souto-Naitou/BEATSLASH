#include "HPComponent.h"
#include <algorithm>

void HPComponent::Initialize(int32_t maxHP)
{
    currentHP_ = maxHP_ =  maxHP;
}

void HPComponent::Damage(int32_t damageAmount)
{
    currentHP_ -= damageAmount;
    currentHP_ = std::max(currentHP_, 0); // HPが0未満にならないようにする
}

float HPComponent::GetHPRation() const
{
    if (maxHP_ > 0)// 0割りを防止
        return static_cast<float>(currentHP_) / static_cast<float>(maxHP_);
    else
        return 0.0f;
}

