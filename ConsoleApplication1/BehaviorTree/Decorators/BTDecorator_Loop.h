// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "BehaviorTree/BTDecorator.h"

struct FBTLoopDecoratorMemory
{
	int32 SearchId;
	uint8 RemainingExecutions;
	float TimeStarted;
};

/**
 * Loop decorator node.
 * A decorator node that bases its condition on whether its loop counter has been exceeded.
 */
class UBTDecorator_Loop : public UBTDecorator
{
public:
	UBTDecorator_Loop();

	mutable int32 SearchId;
	mutable uint8 RemainingExecutions;
	mutable float TimeStarted;

	/** number of executions */
	int32 NumLoops;

	/** infinite loop */
	bool bInfiniteLoop;

	/** timeout (when looping infinitely, when we finish a loop we will check whether we have spent this time looping, if we have we will stop looping). A negative value means loop forever. */
	float InfiniteLoopTimeoutTime;

protected:

	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData);
};
