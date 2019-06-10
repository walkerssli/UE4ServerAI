
#include "BehaviorTree/Tasks/BTTask_AttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTTask_AttackTarget, UBTTask_AttackTarget_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY(UBTTask_AttackTarget, UBTTask_AttackTarget_Factory)

UBTTask_AttackTarget::UBTTask_AttackTarget() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->EnterAttackTarget(this, BlackboardKey.c_str());
    //}

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_AttackTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->ExitAttackTarget(this, BlackboardKey.c_str());
    //}
}

void UBTTask_AttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = (EBTNodeResult::Type)((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickAttackTarget(this, BlackboardKey.c_str());
    //}

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}

EBTNodeResult::Type UBTTask_AttackTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Aborted;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->AbortAttackTarget(this, BlackboardKey.c_str());
    //}

    return (EBTNodeResult::Type)iRet;
}
