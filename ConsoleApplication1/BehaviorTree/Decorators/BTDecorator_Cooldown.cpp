// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Decorators/BTDecorator_Cooldown.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTDecorator_Cooldown, UBTDecorator_Cooldown_Factory, DecoratorNodeFactoryBase)
ADD_ATTRIBUTE(Float, CoolDownTime)
END_DERIVED_NODE_FACTORY(UBTDecorator_Cooldown, UBTDecorator_Cooldown_Factory)

UBTDecorator_Cooldown::UBTDecorator_Cooldown() : UBTDecorator()
{
	NodeName = "Cooldown";
	CoolDownTime = 5.0f;

	// aborting child nodes doesn't makes sense, cooldown starts after leaving this branch
	bAllowAbortChildNodes = false;

	bNotifyTick = false;
	bNotifyDeactivation = true;

	LastUseTimestamp = -FLT_MAX;
	bRequestedRestart = false;
}

void UBTDecorator_Cooldown::PostLoad()
{
	UBTDecorator::PostLoad();
	bNotifyTick = (FlowAbortMode != EBTFlowAbortMode::None);
}

bool UBTDecorator_Cooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const float TimePassed = (FBTTime::GetTimeSeconds() - LastUseTimestamp);
	return TimePassed >= CoolDownTime;
}

void UBTDecorator_Cooldown::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	LastUseTimestamp = FBTTime::GetTimeSeconds();
	bRequestedRestart = false;
}

void UBTDecorator_Cooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bRequestedRestart)
	{
		const float TimePassed = (FBTTime::GetTimeSeconds() - LastUseTimestamp);
		if (TimePassed >= CoolDownTime)
		{
			bRequestedRestart = true;
			OwnerComp.RequestExecution(this);
		}
	}
}
