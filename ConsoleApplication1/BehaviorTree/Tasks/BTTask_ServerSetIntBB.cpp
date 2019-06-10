#include "BehaviorTree/Tasks/BTTask_ServerSetIntBB.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTTask_ServerSetIntBB, UBTTask_ServerSetIntBB_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BBKey)
ADD_ATTRIBUTE(Int, BBValue)
END_DERIVED_NODE_FACTORY(UBTTask_ServerSetIntBB, UBTTask_ServerSetIntBB_Factory)

UBTTask_ServerSetIntBB::UBTTask_ServerSetIntBB() : UBTTaskNode()
{
	NodeName = "SetBlackboardValue";
}

EBTNodeResult::Type UBTTask_ServerSetIntBB::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* bbComp =  OwnerComp.GetBlackboardComponent();
	if (bbComp)
	{
		bbComp->SetValueAsInt(BBKey, BBValue);
	}
	//OwnerComp.RegisterMessageObserver(this, AIMessageType::TestNumIncreased);
	//return EBTNodeResult::InProgress;
	return EBTNodeResult::Succeeded;
}

FString UBTTask_ServerSetIntBB::GetDescription() const
{
	char tmp[256];
	sprintf_s(tmp, 256, "UBTTask_SetBlackboardValue: BlackboardKey:%s NewValue:%d ", BBKey.c_str(), BBValue);
	return tmp;

}
