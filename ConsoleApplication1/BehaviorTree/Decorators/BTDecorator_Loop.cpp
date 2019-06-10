// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BehaviorTree/BTCompositeNode.h"
//#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"

UBTDecorator_Loop::UBTDecorator_Loop() : UBTDecorator()
{
	NodeName = "Loop";
	NumLoops = 3;
	InfiniteLoopTimeoutTime = -1.f;
	bNotifyActivation = true;
	
	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;

	SearchId = 0;
	RemainingExecutions = 0;
	TimeStarted = 0;
}

void UBTDecorator_Loop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	UBTCompositeNode* parent = GetParentNode();
	//const bool bIsSpecialNode = IsANodeType<UBTComposite_SimpleParallel>(parent);
	const bool bIsSpecialNode = false;

	if ((bIsSpecialNode && parent->CurrentChild == BTSpecialChild::NotInitialized) ||
		(!bIsSpecialNode && parent->CurrentChild != ChildIndex))
	{
		// initialize counter if it's first activation
		RemainingExecutions = NumLoops;
		TimeStarted = FBTTime::GetTimeSeconds();
	}

	bool bShouldLoop = false;
	if (bInfiniteLoop)
	{
		// protect from truly infinite loop within single search
		if (SearchData.SearchId != SearchId)
		{
			if ((InfiniteLoopTimeoutTime < 0.f) || ((TimeStarted + InfiniteLoopTimeoutTime) > FBTTime::GetTimeSeconds()))
			{
				bShouldLoop = true;
			}
		}

		SearchId = SearchData.SearchId;
	}
	else
	{
		RemainingExecutions--;
		bShouldLoop = RemainingExecutions > 0;
	}

	UE_VLOG("UBTDecorator_Loop::OnNodeActivation  RemainingExecutions£º%d  shouldLoop:%d \n", RemainingExecutions, (int)bShouldLoop);
	// set child selection overrides
	if (bShouldLoop)
	{
		GetParentNode()->SetChildOverride(SearchData, ChildIndex);
	}
}
