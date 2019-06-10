
#pragma once

#include "BehaviorTree/BTTaskNode.h"

class UBTTask_Talk: public UBTTaskNode
{
public:
    UBTTask_Talk();


    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
    virtual FString GetDescription() const;

    float TalkTime;
    FString Message;

    float RemainingTalkTime;

protected:
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
