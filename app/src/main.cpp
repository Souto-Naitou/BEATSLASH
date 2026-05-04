#include <Windows.h>

#include <framework/RhythmActionGame.h>

#include <memory>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::unique_ptr<RhythmActionGame> game = std::make_unique<RhythmActionGame>();

    game->Run(); 

    return 0;
}