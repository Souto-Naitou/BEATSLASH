#pragma once

#include <Camera.h>
#include <Vector2.h>

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
    void End();
    bool IsActive() const { return isActive_; }

    Tako::Camera* GetCamera() { return pCamera_.get(); }

private:
    std::unique_ptr<Tako::Camera> pCamera_;
    const Tako::Transform* pTarget_ = nullptr;

    Tako::Vector3 startPos_, targetPos_;
    Tako::Vector3 startRot_, targetRot_;
    float elapsed_  = 0.0f;
    float duration_ = 1.0f;
    bool  isActive_ = false;

};