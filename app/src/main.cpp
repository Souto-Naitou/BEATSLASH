#include <Windows.h>

#include <framework/BeatSlash.h>

#include <memory>

#include "../../module/ozSound/audio/SoundEngine.h"

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::unique_ptr<BeatSlash> game = std::make_unique<BeatSlash>();

    game->Run(); 

    return 0;
}