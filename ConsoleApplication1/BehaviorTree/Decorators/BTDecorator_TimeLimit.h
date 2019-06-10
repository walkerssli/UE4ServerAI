// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "BehaviorTree/BTDecorator.h"

/**
 * Time Limit decorator node.
 * A decorator node that bases its condition on whether a timer has exceeded a specified value. The timer is reset each time the node becomes relevant.
 */
class UBTDecorator_TimeLimit : public UBTDecorator
{
public:
    UBTDecorator_TimeLimit();

    /** max allowed time for execution of underlying node */
    float TimeLimit;

protected:
    virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData);
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
