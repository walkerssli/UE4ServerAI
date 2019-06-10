
#include "BehaviorTree/Tasks/BTTask_WaitBlackboardTime.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTTask_WaitBlackboardTime::UBTTask_WaitBlackboardTime() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_WaitBlackboardTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = OwnerComp.GetAgent()->EnterWaitBlackboardTime(this, BlackboardKey.c_str());
    }

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_WaitBlackboardTime::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->ExitWaitBlackboardTime(this, BlackboardKey.c_str());
    }
}

void UBTTask_WaitBlackboardTime::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = (EBTNodeResult::Type)OwnerComp.GetAgent()->TickWaitBlackboardTime(this, BlackboardKey.c_str());
    }

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}
