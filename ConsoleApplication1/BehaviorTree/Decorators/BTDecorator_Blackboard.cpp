// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "math.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
//#include "UObject/Package.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/TdrReflection.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/TdrReflectionErr.h"

UBTDecorator_Blackboard::UBTDecorator_Blackboard() : UBTDecorator_BlackboardBase()
{
	NodeName = "Blackboard Based Condition";
	NotifyObserver = EBTBlackboardRestart::ResultChange;
}

bool UBTDecorator_Blackboard::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	// note that this may produce unexpected logical results. FALSE is a valid return value here as well
	// @todo signal it
	return BlackboardComp && EvaluateOnBlackboard(*BlackboardComp);
}

bool UBTDecorator_Blackboard::EvaluateOnBlackboard(const UBlackboardComponent& BlackboardComp) const
{
	bool bResult = false;
	if (!BlackboardKey.empty())
	{
		int actorID = BlackboardComp.GetActorID();
		FString bbName = BlackboardComp.GetBBName();
		TdrDataTypes dataType = GetDataType(actorID, bbName, BlackboardKey);
		if (dataType == TdrDataTypes_Int)
		{
			EArithmeticKeyOperation::Type operationType = (EArithmeticKeyOperation::Type)OperationType;
			int64 i64Value;
			int ret = GetIntBBValue(i64Value, actorID, bbName, BlackboardKey);
			int Value = (int)i64Value;
			if (ret == Err_BB_OK)
			{
				switch (operationType)
				{
				case EArithmeticKeyOperation::Equal:			return (Value == IntValue);
				case EArithmeticKeyOperation::NotEqual:			return (Value != IntValue);
				case EArithmeticKeyOperation::Less:				return (Value < IntValue);
				case EArithmeticKeyOperation::LessOrEqual:		return (Value <= IntValue);
				case EArithmeticKeyOperation::Greater:			return (Value > IntValue);
				case EArithmeticKeyOperation::GreaterOrEqual:	return (Value >= IntValue);
				}
			}
		}
		else if (dataType == TdrDataTypes_Float)
		{
			EArithmeticKeyOperation::Type operationType = (EArithmeticKeyOperation::Type)OperationType;
			double doubleValue;
			int ret = GetFloatBBValue(doubleValue, actorID, bbName, BlackboardKey);
			float Value = (float)doubleValue;
			if (ret == Err_BB_OK)
			{
				switch (operationType)
				{
				case EArithmeticKeyOperation::Equal:			return fabs(Value - FloatValue) < 0.000001;
				case EArithmeticKeyOperation::NotEqual:			return (Value != FloatValue);
				case EArithmeticKeyOperation::Less:				return (Value < FloatValue);
				case EArithmeticKeyOperation::LessOrEqual:		return (Value <= FloatValue);
				case EArithmeticKeyOperation::Greater:			return (Value > FloatValue);
				case EArithmeticKeyOperation::GreaterOrEqual:	return (Value >= FloatValue);
				}
			}

		}

	}

	return bResult;
}

EBlackboardNotificationResult UBTDecorator_Blackboard::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();
	if (BehaviorComp == nullptr)
	{
		return EBlackboardNotificationResult_RemoveObserver;
	}

	// if (BlackboardKey == ChangedKeyID)
	{
		// can't simply use BehaviorComp->RequestExecution(this) here, we need to support condition/value change modes

		const EBTDecoratorAbortRequest RequestMode = (NotifyObserver == EBTBlackboardRestart::ValueChange) ? ConditionPassing : ConditionResultChanged;
		ConditionalFlowAbort(*BehaviorComp, RequestMode);
	}

	return EBlackboardNotificationResult_ContinueObserving;
}