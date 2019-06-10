#include "BehaviorTree/Tasks/BTTask_Talk.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTTask_Talk, UBTTask_Talk_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Float, TalkTime)
ADD_ATTRIBUTE(Str, Message)
END_DERIVED_NODE_FACTORY(UBTTask_Talk, UBTTask_Talk_Factory)

UBTTask_Talk::UBTTask_Talk() : UBTTaskNode()
{
	NodeName = "Talk";

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Talk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* bbComp =  OwnerComp.GetBlackboardComponent();
	if (bbComp)
	{
		RemainingTalkTime = TalkTime;
		UE_VLOG("UBTTask_Talk:%s\n", GetDescription().c_str());
	}
	return EBTNodeResult::InProgress;
}

FString UBTTask_Talk::GetDescription() const
{
	char tmp[256];
	sprintf_s(tmp, 256, "UBTTask_Talk: Message:%s", Message.c_str());
	return tmp;
}

void UBTTask_Talk::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	RemainingTalkTime -= DeltaSeconds;
	if (RemainingTalkTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
