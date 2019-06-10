// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_BlackboardBase::UBTDecorator_BlackboardBase() : UBTDecorator()
{
	NodeName = "BlackboardBase";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	// empty KeySelector = allow everything
}

void UBTDecorator_BlackboardBase::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		//auto KeyID = BlackboardKey.GetSelectedKeyID();
		BlackboardComp->RegisterObserver(BlackboardKey, this, FOnBlackboardChangeNotification::Create<UBTDecorator_BlackboardBase>(this, &UBTDecorator_BlackboardBase::OnBlackboardKeyValueChange));
	}
}

void UBTDecorator_BlackboardBase::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->UnregisterObserversFrom(this);
	}
}

EBlackboardNotificationResult UBTDecorator_BlackboardBase::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();
	if (BehaviorComp == nullptr)
	{
		return EBlackboardNotificationResult_RemoveObserver;
	}

	if (BlackboardKey == ChangedKeyID)
	{
		BehaviorComp->RequestExecution(this);		
	}
	return EBlackboardNotificationResult_ContinueObserving;
}