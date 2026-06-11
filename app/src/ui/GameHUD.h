#pragma once

#include "ComboUI.h"
#include "RhythmHint.h"
#include "ControlKeyUI.h"
#include "HPBarUI.h"

#include <memory>

#include <combo/ComboBuffSystem.h>
#include <manager/BeatManager.h>
#include <component/HPComponent.h>
#include <character/player/PlayerInput.h>

struct HUDContext
{
    const ComboBuffSystem& comboBuffSystem;
    const BeatClock& beatClock;
    const HPComponent& playerHPComponent;
    const PlayerInput& playerInput;
};

class GameHUD
{
public:

    GameHUD(HUDContext context)        :
        comboBuffSystem_(context.comboBuffSystem),
        beatClock_(context.beatClock),
        playerHPComponent_(context.playerHPComponent),
        playerInput_(context.playerInput)
    {
    }

    void Initialize();
    void Update();
    void Draw();

    void SetBossHPComponent(const HPComponent* pBossHPComponent)
    {
        pBossHPComponent_ = pBossHPComponent;
    }

private:

    std::unique_ptr<ComboUI>        comboUI_;
    std::unique_ptr<RhythmHintUI>   rhythmHintUI_;
    std::unique_ptr<HPBarUI>        playerHPBarUI_;
    std::unique_ptr<HPBarUI>        bossHPBarUI_;
    std::unique_ptr<ControlKeyUI>   controlKeyUI_;

    const ComboBuffSystem& comboBuffSystem_;
    const BeatClock&       beatClock_;
    const HPComponent&     playerHPComponent_;
    const PlayerInput&     playerInput_;

    const HPComponent*     pBossHPComponent_;
};
