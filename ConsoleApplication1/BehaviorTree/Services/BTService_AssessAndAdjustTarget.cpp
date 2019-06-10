
#include "BehaviorTree/Services/BTService_AssessAndAdjustTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTService_AssessAndAdjustTarget::UBTService_AssessAndAdjustTarget() : UBTService()
{
    NodeName = "BTService_AssessAndAdjustTarget";

}

void UBTService_AssessAndAdjustTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->TickAssessAndAdjustTarget(this, BlackboardKey.c_str(), BlackboardKey2.c_str());
    }
}
