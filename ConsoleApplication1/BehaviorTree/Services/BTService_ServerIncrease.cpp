// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Services/BTService_ServerIncrease.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AIMessageTypes.h"
#include "../Blackboard/TdrReflectionErr.h"

UBTService_ServerIncrease::UBTService_ServerIncrease() : UBTService()
{
	NodeName = "BTService_ServerIncrease";
}

void UBTService_ServerIncrease::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBTService::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* bbComp = OwnerComp.GetBlackboardComponent();
	if (bbComp)
	{
		UE_VLOG("UBTService_ServerIncrease::TickNode Name:%s\n", NodeName.c_str());
	}
	else
	{
		return;
	}

	int error = Err_Invalid;
	int ret = bbComp->GetValueAsInt(IncreaseKey, &error);
	if (error == Err_BB_OK)
	{
		//UE_VLOG("%s::TickNode SetValueAsInt:%d\n", mName.c_str(), (ret + 1) % 12);
		bbComp->SetValueAsInt(IncreaseKey, (ret + 1) % 12);
		FAIMessage msg(AIMessageType::TestNumIncreased, true);
		OwnerComp.ReceiveAIMessage(msg);
		UE_VLOG("UBTService_ServerIncrease::TickNode Name:%s BBVal:%d\n", NodeName.c_str(), (ret + 1) % 12);
	}
}
