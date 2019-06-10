
#pragma once

#include "BehaviorTree/BTTaskNode.h"

class UBTTask_MoveToPatrol: public UBTTaskNode
{
public:
    UBTTask_MoveToPatrol();


    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);
    FString BlackboardKey;

protected:
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);


};
