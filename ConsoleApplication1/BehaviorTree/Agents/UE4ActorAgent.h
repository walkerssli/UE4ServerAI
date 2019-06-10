
#pragma once

class UE4ActorAgent
{
public:
    UE4ActorAgent();

    int Init(void* pBTManager);

    int TickMoveToHome(void* Node);

    void TickAccessAndFlee(void* Node, const char* BlackboardKey);

    void ExitMoveToTarget(void* Node, const char* BlackboardKey);

    void ExitMoveToHome(void* Node);

    int TickMoveToFlee(void* Node);

    void ExitMoveToFlee(void* Node);

    int AbortAttackTarget(void* Node, const char* BlackboardKey);

    int EnterMoveToFlee(void* Node);

    int EnterAttackTarget(void* Node, const char* BlackboardKey);

    void ExitAttackTarget(void* Node, const char* BlackboardKey);

    int TickMoveToPatrol(void* Node, const char* BlackboardKey);

    void TickOutOfTraceRangeSettingHome(void* Node, const char* BlackboardKey);

    void OnBecomeRelevantOutOfActiveRangeSettingHome(void* Node, const char* BlackboardKey);

    void TickAssessAndAdjustTarget(void* Node, const char* BlackboardKey, const char* BlackboardKey2);

    void ExitWaitBlackboardTime(void* Node, const char* BlackboardKey);

    int TickAttackTarget(void* Node, const char* BlackboardKey);

    void TickOutOfActiveRangeSettingHome(void* Node, const char* BlackboardKey);

    void TickSearchTarget(void* Node, const char* BlackboardKey);

    void ExitMoveToPatrol(void* Node, const char* BlackboardKey);

    int EnterMoveToHome(void* Node);

    int TickMoveToTarget(void* Node, const char* BlackboardKey);

    int EnterMoveToPatrol(void* Node, const char* BlackboardKey);

    int TickWaitBlackboardTime(void* Node, const char* BlackboardKey);

    void TickAccessAndExitFlee(void* Node, const char* BlackboardKey);

    int EnterWaitBlackboardTime(void* Node, const char* BlackboardKey);

    int EnterMoveToTarget(void* Node, const char* BlackboardKey);

private:
    void* m_pBTManager;
};

