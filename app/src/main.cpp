#include <Windows.h>

#include <framework/RhythmActionGame.h>

#include <memory>

#include "../../module/ozSound/audio/SoundEngine.h"

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::unique_ptr<RhythmActionGame> game = std::make_unique<RhythmActionGame>();

    game->Run(); 

    return 0;
}