// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Decorators/BTDecorator_ConditionalLoop.h"
#include "BehaviorTree/BTCompositeNode.h"

UBTDecorator_ConditionalLoop::UBTDecorator_ConditionalLoop() : UBTDecorator_Blackboard()
{
	NodeName = "Conditional Loop";
	bNotifyDeactivation = true;

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

bool UBTDecorator_ConditionalLoop::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// always allows execution
	return true;
}

EBlackboardNotificationResult UBTDecorator_ConditionalLoop::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	// empty, don't react to blackboard value changes
	return EBlackboardNotificationResult_RemoveObserver;
}

void UBTDecorator_ConditionalLoop::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	if (NodeResult != EBTNodeResult::Aborted)
	{
		const UBlackboardComponent* BlackboardComp = SearchData.OwnerComp.GetBlackboardComponent();
		const bool bEvalResult = BlackboardComp && EvaluateOnBlackboard(*BlackboardComp);

		if (bEvalResult != IsInversed())
		{
			GetParentNode()->SetChildOverride(SearchData, GetChildIndex());
		}
	}
}
