
#pragma once

#include "BehaviorTree/BTService.h"

class UBTService_OutOfActiveRangeSettingHome: public UBTService
{
public:
    UBTService_OutOfActiveRangeSettingHome();
    FString BlackboardKey;

    /** called when auxiliary node becomes active
     * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:

    /** update next tick interval
    * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
