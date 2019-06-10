
#include "BehaviorTree/Services/BTService_AssessAndAdjustTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTService_AssessAndAdjustTarget, UBTService_AssessAndAdjustTarget_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
ADD_ATTRIBUTE(Str, BlackboardKey2)
END_DERIVED_NODE_FACTORY(UBTService_AssessAndAdjustTarget, UBTService_AssessAndAdjustTarget_Factory)
 
UBTService_AssessAndAdjustTarget::UBTService_AssessAndAdjustTarget() : UBTService()
{
    NodeName = "BTService_AssessAndAdjustTarget";

}

void UBTService_AssessAndAdjustTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickAssessAndAdjustTarget(this, BlackboardKey.c_str(), BlackboardKey2.c_str());
    //}
}
