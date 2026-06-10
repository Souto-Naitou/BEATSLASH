#include "BossBTNodeRegistration.h"
#include <BTNodeRegistry.h>
#include "BTWaitAction.h"
#include "BTRandomMoveAction.h"
#include "BTChargeRazer.h"

void RegisterBossBTNodes()
{
    auto* registry = Tako::BTNodeRegistry::GetInstance();

    // 標準コンポジット（Selector/Sequence/RandomSelector/Parallel）の登録
    registry->Initialize();

    registry->RegisterNode<BTWaitAction>("BTWaitAction",
        Tako::NodeMeta{ "Wait", Tako::NodeCategory::Action, Tako::NodeColor(0.8f, 0.8f, 0.3f, 1.0f), false });
    registry->RegisterNode<BTRandomMoveAction>("BTRandomMoveAction",
        Tako::NodeMeta{ "RandomMove", Tako::NodeCategory::Action, Tako::NodeColor(0.3f, 0.8f, 0.8f, 1.0f), false });
    registry->RegisterNode<BTChargeRazer>("BTChargeRazer",
        Tako::NodeMeta{ "ChargeRazer", Tako::NodeCategory::Action, Tako::NodeColor(0.9f, 0.3f, 0.3f, 1.0f), false });
}
