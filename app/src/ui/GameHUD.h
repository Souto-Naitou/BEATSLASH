#pragma once

#include "ComboUI.h"
#include "RhythmHint.h"

#include <memory>

#include <combo/ComboBuffSystem.h>
#include <manager/BeatManager.h>
#include <component/HPComponent.h>
#include "PlayerHPBarUI.h"

class GameHUD
{
public:

    GameHUD(const ComboBuffSystem& comboBuffSystem, const BeatClock& beatClock,const HPComponent& hpComponent) :
        comboBuffSystem_(comboBuffSystem),
        beatClock_(beatClock),
        hpComponent_(hpComponent)
    {
    }

    void Initialize();
    void Update();
    void Draw();

private:
    
    std::unique_ptr<ComboUI> comboUI_;
    std::unique_ptr<RhythmHintUI> rhythmHintUI_;
    std::unique_ptr<PlayerHPBarUI> playerHPBarUI_;

    // コンボバフシステムの参照
    const ComboBuffSystem& comboBuffSystem_;
    const BeatClock& beatClock_;
    const HPComponent& hpComponent_;
};