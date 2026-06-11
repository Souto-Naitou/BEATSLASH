#pragma once

#include <OBBCollider.h>
#include <functional>
#include <character/player/ParryJudgement.h>

using PlayerPushBackCallback = std::function<void(const Tako::Vector3& pushback)>;
using PlayerParrySuccessCallback = std::function<void()>;

class ComboBuffSystem;
class HPComponent;

class PlayerCollider : public Tako::OBBCollider
{
public:
    struct InitData
    {
        PlayerPushBackCallback pushBackCallback;
        PlayerParrySuccessCallback parrySuccessCallback;
        ComboBuffSystem& comboBuffSystem;
        HPComponent& hpComponent;
        ParryJudgement& parryJudgement;
    };

    PlayerCollider (const InitData& initData) :
        pushBackCallback_(initData.pushBackCallback),
        parrySuccessCallback_(initData.parrySuccessCallback),
        comboBuffSystem_(initData.comboBuffSystem),
        hpComponent_(initData.hpComponent),
        parryJudgement_(initData.parryJudgement)
    {
    }


    void OnCollisionEnter(Collider* other) override;


    void OnCollisionStay(Collider* other) override;


    void OnCollisionExit(Collider* other) override;

private:
    PlayerPushBackCallback pushBackCallback_;
    PlayerParrySuccessCallback parrySuccessCallback_;
    ComboBuffSystem& comboBuffSystem_; // コンボシステムの参照
    HPComponent& hpComponent_; // HPコンポーネントの参照
    const ParryJudgement& parryJudgement_; // 攻撃ヒット記録クラスの参照
};