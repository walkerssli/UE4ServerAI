
#include "BehaviorTree/Tasks/BTTask_MoveToFlee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTTask_MoveToFlee, UBTTask_MoveToFlee_Factory, TaskNodeFactoryBase)
END_DERIVED_NODE_FACTORY(UBTTask_MoveToFlee, UBTTask_MoveToFlee_Factory)

UBTTask_MoveToFlee::UBTTask_MoveToFlee() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->EnterMoveToFlee(this);
    //}

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_MoveToFlee::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->ExitMoveToFlee(this);
    //}
}

void UBTTask_MoveToFlee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = (EBTNodeResult::Type)((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickMoveToFlee(this);
    //}

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}
