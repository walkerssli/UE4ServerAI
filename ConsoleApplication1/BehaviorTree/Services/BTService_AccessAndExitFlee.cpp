
#include "BehaviorTree/Services/BTService_AccessAndExitFlee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTService_AccessAndExitFlee::UBTService_AccessAndExitFlee() : UBTService()
{
    NodeName = "BTService_AccessAndExitFlee";

}

void UBTService_AccessAndExitFlee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->TickAccessAndExitFlee(this, BlackboardKey.c_str());
    }
}
