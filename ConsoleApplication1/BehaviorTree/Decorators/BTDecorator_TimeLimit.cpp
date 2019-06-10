// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Decorators/BTDecorator_TimeLimit.h"

UBTDecorator_TimeLimit::UBTDecorator_TimeLimit() : UBTDecorator()
{
	NodeName = "TimeLimit";
	TimeLimit = 5.0f;
	bNotifyActivation = true;
	bNotifyTick = true;
	bTickIntervals = true;

	// time limit always abort current branch
	bAllowAbortLowerPri = false;
	bAllowAbortNone = false;
	FlowAbortMode = EBTFlowAbortMode::Self;
}

void UBTDecorator_TimeLimit::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	NextTickRemainingTime = TimeLimit;
	AccumulatedDeltaTime = 0.0f;
}

void UBTDecorator_TimeLimit::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_VLOG("UBTDecorator_TimeLimit::TickNode  RequestExecution \n");
	OwnerComp.RequestExecution(this);
}

// BnS End
//////////////////////////////////////////////////////////////////////////