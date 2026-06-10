#pragma once

#include "ComboUI.h"
#include "RhythmHint.h"

#include <memory>

#include <combo/ComboBuffSystem.h>
#include <manager/BeatManager.h>

class GameHUD
{
public:

    GameHUD(const ComboBuffSystem& comboBuffSystem, const BeatClock& beatClock);

    void Initialize();
    void Update();
    void Draw();




private:
    
    std::unique_ptr<ComboUI> comboUI_;
    std::unique_ptr<RhythmHintUI> rhythmHintUI_;

    // コンボバフシステムの参照
    const ComboBuffSystem& comboBuffSystem_;
    const BeatClock& beatClock_;
};