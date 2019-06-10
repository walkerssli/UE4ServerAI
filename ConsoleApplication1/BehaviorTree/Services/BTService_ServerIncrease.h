// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "BehaviorTree/BTService.h"

class UBTService_ServerIncrease: public UBTService
{
public:
    UBTService_ServerIncrease();

protected:

    /** update next tick interval
    * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

public:
    FString IncreaseKey;
};
