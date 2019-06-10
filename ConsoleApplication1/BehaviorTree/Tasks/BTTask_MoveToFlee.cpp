
#include "BehaviorTree/Tasks/BTTask_MoveToFlee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTTask_MoveToFlee::UBTTask_MoveToFlee() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = OwnerComp.GetAgent()->EnterMoveToFlee(this);
    }

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_MoveToFlee::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->ExitMoveToFlee(this);
    }
}

void UBTTask_MoveToFlee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = (EBTNodeResult::Type)OwnerComp.GetAgent()->TickMoveToFlee(this);
    }

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}
