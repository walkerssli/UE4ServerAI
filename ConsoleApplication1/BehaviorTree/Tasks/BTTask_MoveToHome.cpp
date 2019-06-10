
#include "BehaviorTree/Tasks/BTTask_MoveToHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTTask_MoveToHome::UBTTask_MoveToHome() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = OwnerComp.GetAgent()->EnterMoveToHome(this);
    }

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_MoveToHome::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->ExitMoveToHome(this);
    }
}

void UBTTask_MoveToHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = (EBTNodeResult::Type)OwnerComp.GetAgent()->TickMoveToHome(this);
    }

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}
