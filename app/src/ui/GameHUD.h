#pragma once

#include "ComboUI.h"
#include "RhythmHint.h"
#include "ControlKeyUI.h"
#include "PlayerHPBarUI.h"

#include <memory>

#include <combo/ComboBuffSystem.h>
#include <manager/BeatManager.h>
#include <component/HPComponent.h>
#include <character/player/PlayerInput.h>

class GameHUD
{
public:

    GameHUD(const ComboBuffSystem& comboBuffSystem, const BeatClock& beatClock,
            const HPComponent& hpComponent, const PlayerInput& playerInput) :
        comboBuffSystem_(comboBuffSystem),
        beatClock_(beatClock),
        hpComponent_(hpComponent),
        playerInput_(playerInput)
    {
    }

    void Initialize();
    void Update();
    void Draw();

private:

    std::unique_ptr<ComboUI>        comboUI_;
    std::unique_ptr<RhythmHintUI>   rhythmHintUI_;
    std::unique_ptr<PlayerHPBarUI>  playerHPBarUI_;
    std::unique_ptr<ControlKeyUI>   controlKeyUI_;

    const ComboBuffSystem& comboBuffSystem_;
    const BeatClock&       beatClock_;
    const HPComponent&     hpComponent_;
    const PlayerInput&     playerInput_;
};
