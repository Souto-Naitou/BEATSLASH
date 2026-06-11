#include "BeatEdgeDetector.h"
#include <manager/BeatManager.h>
#include <BTBlackboard.h>
#include <character/boss/bt/BossBlackboardKeys.h>

void BeatEdgeDetector::Reset()
{
    lastBeatIndex_ = -1;
    beatPassed_ = false;
}

bool BeatEdgeDetector::Update(const BeatClock* beatClock, Tako::BTBlackboard* blackboard)
{
    beatPassed_ = false;

    if (beatClock)
    {
        int currentBeatIndex = beatClock->GetCurrentBeatIndex();

        // 初回は基準値の記録のみ行う
        if (lastBeatIndex_ == -1)
        {
            lastBeatIndex_ = currentBeatIndex;
        }

        beatPassed_ = (currentBeatIndex > lastBeatIndex_);
        lastBeatIndex_ = currentBeatIndex;

        if (blackboard)
        {
            blackboard->SetValue<bool>(BossBlackboardKeys::kBeatPassed, beatPassed_);
            blackboard->SetInt(BossBlackboardKeys::kCurrentBeatIndex, currentBeatIndex);
            blackboard->SetFloat(BossBlackboardKeys::kSecondsPerBeat, beatClock->GetSecondsPerBeat());
        }
    }

    return beatPassed_;
}
