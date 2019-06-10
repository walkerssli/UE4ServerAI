
#include "BehaviorTree/Services/BTService_AccessAndFlee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTService_AccessAndFlee, UBTService_AccessAndFlee_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY(UBTService_AccessAndFlee, UBTService_AccessAndFlee_Factory)
 
UBTService_AccessAndFlee::UBTService_AccessAndFlee() : UBTService()
{
    NodeName = "BTService_AccessAndFlee";

}

void UBTService_AccessAndFlee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickAccessAndFlee(this, BlackboardKey.c_str());
    //}
}
