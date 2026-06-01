#include "RadialBeat.h"
#include <presentation/animation/AnimationTween.hpp>
#include <math/Easing.h>
#include <PostEffectManager.h>
#include <debug/DebugRegisterer.h>


void RadialBeat::Initialize()
{
    DebugRegister("Radial Beat", &AnimationTimeline<float>::ImGui, &timeline_);
    Tako::PostEffectManager::GetInstance()->AddEffectToChain("RadialBlur");
}

void RadialBeat::Finalize()
{
}

void RadialBeat::Start(float duration)
{
    AnimationTween<float> tween0(0.0f, duration * 0.1f, 0.0f, 1.0f);
    tween0.SetTransitionFunction(&Math::Easing::EaseInQuad);
    AnimationTween<float> tween1(duration * 0.1f, duration * 0.9f, 1.0f, 0.0f);
    tween1.SetTransitionFunction(&Math::Easing::EaseOutQuad);
    timeline_.ClearTween();
    timeline_.AddTween(tween0);
    timeline_.AddTween(tween1);
    timeline_.Start();
    isPlaying_ = true;
}

void RadialBeat::Update()
{
    if (!isPlaying_) return;

    const float value = timeline_.Update();
    Tako::RadialBlurParam param = {};
    param.center = { 0.5f, 0.5f };
    param.sampleCount = samples_;
    param.blurWidth = maxWidth_ * value;
    Tako::PostEffectManager::GetInstance()->SetEffectParam("RadialBlur", param);
}
