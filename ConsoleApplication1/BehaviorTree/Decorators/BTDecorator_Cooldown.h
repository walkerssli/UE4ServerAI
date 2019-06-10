// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BehaviorTree/BTDecorator.h"

struct FBTCooldownDecoratorMemory
{
};

/**
 * Cooldown decorator node.
 * A decorator node that bases its condition on whether a cooldown timer has expired.
 */
class UBTDecorator_Cooldown : public UBTDecorator
{
public:
	UBTDecorator_Cooldown();

	mutable float LastUseTimestamp;
	mutable uint8 bRequestedRestart;

	/** max allowed time for execution of underlying node */
	float CoolDownTime;

	//~ Begin UObject Interface
	virtual void PostLoad();
	//~ End UObject Interface

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

protected:

	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult);
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
