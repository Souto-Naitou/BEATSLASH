#include "BossBTNodeRegistration.h"
#include <BTNodeRegistry.h>
#include "BTWaitAction.h"
#include "BTRandomMoveAction.h"
#include "BTChargeRazer.h"
#include "BTApproachAction.h"
#include "BTRetreatAction.h"
#include "BTDistanceCondition.h"
#include "BTMeleeAttackAction.h"

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
    registry->RegisterNode<BTApproachAction>("BTApproachAction",
        Tako::NodeMeta{ "Approach", Tako::NodeCategory::Action, Tako::NodeColor(0.3f, 0.8f, 0.4f, 1.0f), false });
    registry->RegisterNode<BTRetreatAction>("BTRetreatAction",
        Tako::NodeMeta{ "Retreat", Tako::NodeCategory::Action, Tako::NodeColor(0.7f, 0.4f, 0.9f, 1.0f), false });
    registry->RegisterNode<BTDistanceCondition>("BTDistanceCondition",
        Tako::NodeMeta{ "DistanceCheck", Tako::NodeCategory::Condition, Tako::NodeColor(0.9f, 0.6f, 0.2f, 1.0f), false });
    registry->RegisterNode<BTMeleeAttackAction>("BTMeleeAttackAction",
        Tako::NodeMeta{ "MeleeAttack", Tako::NodeCategory::Action, Tako::NodeColor(0.9f, 0.45f, 0.15f, 1.0f), false });
}
