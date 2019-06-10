
#pragma once

#include "BehaviorTree/BTService.h"

class UBTService_AccessAndFlee: public UBTService
{
public:
    UBTService_AccessAndFlee();
    FString BlackboardKey;

protected:

    /** update next tick interval
    * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
