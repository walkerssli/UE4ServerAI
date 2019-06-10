// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"

class UBlackboardComponent;

/**
 * Conditional loop decorator node.
 * A decorator node that loops execution as long as condition is satisfied.
 */
class UBTDecorator_ConditionalLoop : public UBTDecorator_Blackboard
{
public:
    UBTDecorator_ConditionalLoop();

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
    virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
    virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult);

};
