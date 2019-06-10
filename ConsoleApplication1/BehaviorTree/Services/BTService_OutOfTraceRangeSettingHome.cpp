
#include "BehaviorTree/Services/BTService_OutOfTraceRangeSettingHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTService_OutOfTraceRangeSettingHome::UBTService_OutOfTraceRangeSettingHome() : UBTService()
{
    NodeName = "BTService_OutOfTraceRangeSettingHome";

}

void UBTService_OutOfTraceRangeSettingHome::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->TickOutOfTraceRangeSettingHome(this, BlackboardKey.c_str());
    }
}
