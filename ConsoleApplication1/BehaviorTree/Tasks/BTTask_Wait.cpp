// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Tasks/BTTask_Wait.h"

UBTTask_Wait::UBTTask_Wait() : UBTTaskNode()
{
	NodeName = "Wait";
	WaitTime = 5.0f;
	bNotifyTick = true;
	RemainingWaitTime = 0.0f;
}

EBTNodeResult::Type UBTTask_Wait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));

	return EBTNodeResult::InProgress;
}

void UBTTask_Wait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	RemainingWaitTime -= DeltaSeconds;
	UE_VLOG("UBTTask_Wait::TickTask RemainingWaitTime:%.2f\n", RemainingWaitTime);
	if (RemainingWaitTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_Wait::GetInstanceMemorySize() const
{
	return 0;// sizeof(FBTWaitTaskMemory);
}

FString UBTTask_Wait::GetDescription() const
{
	char tmp[256];
	sprintf_s(tmp, 256, "BTTask_Wait: WaitTime:%.2f RandomDeviation:%.2f RemainingWaitTime:%.2f", WaitTime, RandomDeviation, RemainingWaitTime);
	return tmp;
}
