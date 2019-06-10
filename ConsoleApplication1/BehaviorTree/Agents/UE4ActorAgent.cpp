
#include "BehaviorTree/Agents/UE4ActorAgent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

UE4ActorAgent::UE4ActorAgent()
{
    m_pBTManager = NULL;
}

int UE4ActorAgent::Init(void* pBTManager)
{
    m_pBTManager = pBTManager;

    return 0;
}

int UE4ActorAgent::TickMoveToHome(void* Node)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

void UE4ActorAgent::TickAccessAndFlee(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

void UE4ActorAgent::ExitMoveToTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

void UE4ActorAgent::ExitMoveToHome(void* Node)
{
    if (m_pBTManager)
    {

    }
}

int UE4ActorAgent::TickMoveToFlee(void* Node)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

void UE4ActorAgent::ExitMoveToFlee(void* Node)
{
    if (m_pBTManager)
    {

    }
}

int UE4ActorAgent::AbortAttackTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

int UE4ActorAgent::EnterMoveToFlee(void* Node)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

int UE4ActorAgent::EnterAttackTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

void UE4ActorAgent::ExitAttackTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

int UE4ActorAgent::TickMoveToPatrol(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

void UE4ActorAgent::TickOutOfTraceRangeSettingHome(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

void UE4ActorAgent::OnBecomeRelevantOutOfActiveRangeSettingHome(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

void UE4ActorAgent::TickAssessAndAdjustTarget(void* Node, const char* BlackboardKey, const char* BlackboardKey2)
{
    if (m_pBTManager)
    {

    }
}

void UE4ActorAgent::ExitWaitBlackboardTime(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

int UE4ActorAgent::TickAttackTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

void UE4ActorAgent::TickOutOfActiveRangeSettingHome(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

void UE4ActorAgent::TickSearchTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

void UE4ActorAgent::ExitMoveToPatrol(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

int UE4ActorAgent::EnterMoveToHome(void* Node)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

int UE4ActorAgent::TickMoveToTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

int UE4ActorAgent::EnterMoveToPatrol(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

int UE4ActorAgent::TickWaitBlackboardTime(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

void UE4ActorAgent::TickAccessAndExitFlee(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }
}

int UE4ActorAgent::EnterWaitBlackboardTime(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}

int UE4ActorAgent::EnterMoveToTarget(void* Node, const char* BlackboardKey)
{
    if (m_pBTManager)
    {

    }

    return EBTNodeResult::Succeeded;
}
