
#pragma once

#include "BehaviorTree/BTTaskNode.h"

class UBTTask_ServerSetIntBB: public UBTTaskNode
{
public:
    UBTTask_ServerSetIntBB();


    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
    virtual FString GetDescription() const;

    FString BBKey;
    int32 BBValue;

};
