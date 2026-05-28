#pragma once

#include "ComboUI.h"

#include <memory>

#include <combo/ComboBuffSystem.h>

class GameHUD
{
public:

    GameHUD(const ComboBuffSystem& comboBuffSystem);

    void Initialize();
    void Update();
    void Draw();




private:
    
    std::unique_ptr<ComboUI> comboUI_;

    // コンボバフシステムの参照
    const ComboBuffSystem& comboBuffSystem_;
};