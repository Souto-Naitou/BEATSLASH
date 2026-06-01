#pragma once

#include <entity/camera/FocusCamera.h>
#include <entity/camera/FollowCamera.h>
#include <debug/GameParameter.h>

#include <functional>
#include <memory>

class CameraDirector
{
public:

    void Initialize();
    void Update(float deltaTime);

    void SetFollowTarget(const Tako::Transform* target);

    void StartFocus(const Tako::Transform& target, float duration);

    // 補間完了時のコールバック（扉アニメを開始するために使う）
    void SetOnFocusArrived(std::function<void()> cb) { onFocusArrived_ = std::move(cb); }

    // 扉アニメ完了後に外から呼ぶ → 戻り補間を開始する
    void NotifyDoorOpenFinished();

    bool IsFocusing() const { return state_ == State::Focusing; }

    FollowCamera* GetFollowCamera() { return pFollowCamera_.get(); }

private:

    void Activate(Tako::Camera* cam);

private:
    enum class State
    {
        Follow,
        Focusing,
        Waiting,
        Returning,
    } state_ = State::Follow;

    std::unique_ptr<FocusCamera>  pFocusCamera_;
    std::unique_ptr<FollowCamera> pFollowCamera_;

    std::function<void()> onFocusArrived_;

    EnableDebug("Camera Director");
    GameParameter(float, kReturnDuration_, 1.5f);   // 戻り補間の時間（秒）
};
