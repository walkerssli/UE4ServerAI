
#include "BehaviorTree/Services/BTService_AccessAndExitFlee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTService_AccessAndExitFlee, UBTService_AccessAndExitFlee_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY(UBTService_AccessAndExitFlee, UBTService_AccessAndExitFlee_Factory)
 
UBTService_AccessAndExitFlee::UBTService_AccessAndExitFlee() : UBTService()
{
    NodeName = "BTService_AccessAndExitFlee";

}

void UBTService_AccessAndExitFlee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickAccessAndExitFlee(this, BlackboardKey.c_str());
    //}
}
