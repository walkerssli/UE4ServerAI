
#include "BehaviorTree/Services/BTService_OutOfActiveRangeSettingHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTService_OutOfActiveRangeSettingHome, UBTService_OutOfActiveRangeSettingHome_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY(UBTService_OutOfActiveRangeSettingHome, UBTService_OutOfActiveRangeSettingHome_Factory)
 
UBTService_OutOfActiveRangeSettingHome::UBTService_OutOfActiveRangeSettingHome() : UBTService()
{
    NodeName = "BTService_OutOfActiveRangeSettingHome";

    bNotifyBecomeRelevant = true;

}

void UBTService_OutOfActiveRangeSettingHome::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickOutOfActiveRangeSettingHome(this, BlackboardKey.c_str());
    //}
}

void UBTService_OutOfActiveRangeSettingHome::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->OnBecomeRelevantOutOfActiveRangeSettingHome(this, BlackboardKey.c_str());
    //}
}
