// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

class FBlackboardDecoratorDetails;
class UBehaviorTree;
class UBlackboardComponent;

/**
 *  Decorator for accessing blackboard values
 */

namespace EBTBlackboardRestart
{
enum Type
{
	ValueChange = 0,
	ResultChange,
};
}

/**
 * Blackboard decorator node.
 * A decorator node that bases its condition on a Blackboard key.
 */
//UCLASS(HideCategories=(Condition))
class  UBTDecorator_Blackboard : public UBTDecorator_BlackboardBase
{
public:
	UBTDecorator_Blackboard();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

public:

	/** value for arithmetic operations */
	int32 IntValue;

	/** value for arithmetic operations */
	float FloatValue;

	/** value for string operations */
	FString StringValue;

	/** cached description */
	FString CachedDescription;

	/** operation type */
	uint8 OperationType;

	/** when observer can try to request abort? */
	EBTBlackboardRestart::Type NotifyObserver;

	/** take blackboard value and evaluate decorator's condition */
	bool EvaluateOnBlackboard(const UBlackboardComponent& BlackboardComp) const;

};
