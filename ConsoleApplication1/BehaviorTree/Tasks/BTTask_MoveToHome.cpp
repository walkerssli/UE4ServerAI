
#include "BehaviorTree/Tasks/BTTask_MoveToHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTTask_MoveToHome, UBTTask_MoveToHome_Factory, TaskNodeFactoryBase)
END_DERIVED_NODE_FACTORY(UBTTask_MoveToHome, UBTTask_MoveToHome_Factory)

UBTTask_MoveToHome::UBTTask_MoveToHome() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->EnterMoveToHome(this);
    //}

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_MoveToHome::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->ExitMoveToHome(this);
    //}
}

void UBTTask_MoveToHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = (EBTNodeResult::Type)((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickMoveToHome(this);
    //}

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}
