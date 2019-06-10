// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once
//
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTDecorator.h"

class UBehaviorTree;
class UBlackboardComponent;

class UBTDecorator_BlackboardBase : public UBTDecorator
{
public:
	UBTDecorator_BlackboardBase();

	/** notify about change in blackboard keys */
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	
	/** get name of selected blackboard key */
	inline FName GetSelectedBlackboardKey() const
	{
		return BlackboardKey;
	}


	FBlackboardKeySelector BlackboardKey;

	/** called when execution flow controller becomes active */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	/** called when execution flow controller becomes inactive */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
