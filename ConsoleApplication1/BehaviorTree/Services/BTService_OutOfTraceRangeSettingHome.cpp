
#include "BehaviorTree/Services/BTService_OutOfTraceRangeSettingHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTService_OutOfTraceRangeSettingHome, UBTService_OutOfTraceRangeSettingHome_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY(UBTService_OutOfTraceRangeSettingHome, UBTService_OutOfTraceRangeSettingHome_Factory)
 
UBTService_OutOfTraceRangeSettingHome::UBTService_OutOfTraceRangeSettingHome() : UBTService()
{
    NodeName = "BTService_OutOfTraceRangeSettingHome";

}

void UBTService_OutOfTraceRangeSettingHome::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickOutOfTraceRangeSettingHome(this, BlackboardKey.c_str());
    //}
}
