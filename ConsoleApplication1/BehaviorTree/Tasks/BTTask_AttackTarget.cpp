
#include "BehaviorTree/Tasks/BTTask_AttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Agents/UE4ActorAgent.h"

UBTTask_AttackTarget::UBTTask_AttackTarget() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = OwnerComp.GetAgent()->EnterAttackTarget(this, BlackboardKey.c_str());
    }

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_AttackTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (OwnerComp.GetAgent())
    {
        OwnerComp.GetAgent()->ExitAttackTarget(this, BlackboardKey.c_str());
    }
}

void UBTTask_AttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    if (OwnerComp.GetAgent())
    {
        iRet = (EBTNodeResult::Type)OwnerComp.GetAgent()->TickAttackTarget(this, BlackboardKey.c_str());
    }

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}

EBTNodeResult::Type UBTTask_AttackTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Aborted;
    if (OwnerComp.GetAgent())
    {
        iRet = OwnerComp.GetAgent()->AbortAttackTarget(this, BlackboardKey.c_str());
    }

    return (EBTNodeResult::Type)iRet;
}
