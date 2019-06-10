
#include "BehaviorTree/Tasks/BTTask_MoveToPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTTask_MoveToPatrol::UBTTask_MoveToPatrol() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = OwnerComp.GetAgent()->EnterMoveToPatrol(this, BlackboardKey.c_str());
    }

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_MoveToPatrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->ExitMoveToPatrol(this, BlackboardKey.c_str());
    }
}

void UBTTask_MoveToPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = (EBTNodeResult::Type)OwnerComp.GetAgent()->TickMoveToPatrol(this, BlackboardKey.c_str());
    }

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}
