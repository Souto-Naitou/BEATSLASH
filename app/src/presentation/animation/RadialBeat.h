#pragma once
#include <presentation/animation/AnimationTimeline.hpp>
#include <memory>

class RadialBeat
{
public:
    void Initialize();
    void Finalize();
    void Start(float duration);
    void SetMaxWidth(float width) { maxWidth_ = width; }
    void SetSamples(int samples) { samples_ = samples; }
    void Update();

private:
    AnimationTimeline<float> timeline_;
    int samples_ = 4;
    float maxWidth_ = 0.0f;
    bool isPlaying_ = false;
};