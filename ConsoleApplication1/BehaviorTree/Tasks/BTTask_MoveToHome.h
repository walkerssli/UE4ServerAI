
#pragma once

#include "BehaviorTree/BTTaskNode.h"

class UBTTask_MoveToHome: public UBTTaskNode
{
public:
    UBTTask_MoveToHome();


    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);

protected:
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);


};
