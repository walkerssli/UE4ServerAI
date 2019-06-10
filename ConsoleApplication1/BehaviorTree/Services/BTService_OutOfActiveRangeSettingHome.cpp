
#include "BehaviorTree/Services/BTService_OutOfActiveRangeSettingHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTService_OutOfActiveRangeSettingHome::UBTService_OutOfActiveRangeSettingHome() : UBTService()
{
    NodeName = "BTService_OutOfActiveRangeSettingHome";

    bNotifyBecomeRelevant = true;

}

void UBTService_OutOfActiveRangeSettingHome::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->TickOutOfActiveRangeSettingHome(this, BlackboardKey.c_str());
    }
}

void UBTService_OutOfActiveRangeSettingHome::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->OnBecomeRelevantOutOfActiveRangeSettingHome(this, BlackboardKey.c_str());
    }
}
