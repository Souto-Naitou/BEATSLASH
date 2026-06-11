#pragma once

/// <summary>
/// ボスBTのブラックボード共有キー
/// GetPtr/GetValueは格納時と型(const修飾含む)が厳密一致する必要があるため、各キーの格納型をコメントで明記する
/// </summary>
namespace BossBlackboardKeys
{
    inline constexpr const char* kBoss = "boss";                          // Boss*
    inline constexpr const char* kTarget = "target";                      // const ICharacter*
    inline constexpr const char* kBeatClock = "beatClock";                // const BeatClock*
    inline constexpr const char* kEmitterManager = "emitterManager";      // Tako::EmitterManager*
    inline constexpr const char* kBeatPassed = "beatPassed";              // bool（拍境界を跨いだフレームのみtrue）
    inline constexpr const char* kCurrentBeatIndex = "currentBeatIndex";  // int
    inline constexpr const char* kSecondsPerBeat = "secondsPerBeat";      // float
}
