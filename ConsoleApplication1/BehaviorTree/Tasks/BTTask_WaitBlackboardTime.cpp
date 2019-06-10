
#include "BehaviorTree/Tasks/BTTask_WaitBlackboardTime.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTTask_WaitBlackboardTime, UBTTask_WaitBlackboardTime_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY(UBTTask_WaitBlackboardTime, UBTTask_WaitBlackboardTime_Factory)

UBTTask_WaitBlackboardTime::UBTTask_WaitBlackboardTime() : UBTTaskNode()
{
    NodeName = "Talk";

    bNotifyTick = true;

    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_WaitBlackboardTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    int iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->EnterWaitBlackboardTime(this, BlackboardKey.c_str());
    //}

    return (EBTNodeResult::Type)iRet;
}

void UBTTask_WaitBlackboardTime::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    //if (OwnerComp.GetBTManager())
    //{
    //    ((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->ExitWaitBlackboardTime(this, BlackboardKey.c_str());
    //}
}

void UBTTask_WaitBlackboardTime::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    EBTNodeResult::Type iRet = EBTNodeResult::Succeeded;
    //if (OwnerComp.GetBTManager())
    //{
    //    iRet = (EBTNodeResult::Type)((MAYEX::CUE4BTManager*)(OwnerComp.GetBTManager()))->TickWaitBlackboardTime(this, BlackboardKey.c_str());
    //}

    if (iRet != EBTNodeResult::InProgress)
    {
        FinishLatentTask(OwnerComp, iRet);
    }
}
