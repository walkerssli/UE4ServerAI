
#include "BehaviorTree/Services/BTService_AccessAndFlee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTService_AccessAndFlee::UBTService_AccessAndFlee() : UBTService()
{
    NodeName = "BTService_AccessAndFlee";

}

void UBTService_AccessAndFlee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->TickAccessAndFlee(this, BlackboardKey.c_str());
    }
}
