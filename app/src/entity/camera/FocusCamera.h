#pragma once

#include <Camera.h>
#include <Vector2.h>
#include <debug/GameParameter.h>
#include <memory>

class FocusCamera
{
public:
    void Initialize();
    void Update(float deltaTime);

    // 現在のカメラ状態から target へ補間開始
    void Start(const Tako::Vector3& fromPos,
               const Tako::Vector3& fromRot,
               const Tako::Transform& target,
               float duration);
    // FocusCamera の現在位置から指定 pos/rot へ補間（戻り用）
    void StartReturn(const Tako::Vector3& toPos, const Tako::Vector3& toRot, float duration);
    bool IsFinished() const { return elapsed_ >= duration_; }
    void End();
    bool IsActive() const { return isActive_; }

    Tako::Camera* GetCamera() { return pCamera_.get(); }

private:
    std::unique_ptr<Tako::Camera> pCamera_;

    Tako::Vector3 startPos_, targetPos_;
    Tako::Vector3 startRot_, targetRot_;
    float elapsed_  = 0.0f;
    float duration_ = 1.0f;
    bool  isActive_ = false;

    EnableDebug("Focus Camera");
    GameParameter(float, kFocusDistance_, 20.0f);      // 扉からの水平距離
    GameParameter(float, kElevationAngle_,25.0f);     // 見下ろし角（度）
    GameParameter(Tako::Vector3, targetOffset_, Tako::Vector3(0.1f, 5.0f, 0.0f));  // 注視点オフセット

};