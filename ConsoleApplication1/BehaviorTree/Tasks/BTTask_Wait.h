// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once
//
// #include "CoreMinimal.h"
// #include "UObject/ObjectMacros.h"
#include "BehaviorTree/BTTaskNode.h"
//#include "BTTask_Wait.generated.h"
//
// struct FBTWaitTaskMemory
// {
// 	/** time left */
// 	float RemainingWaitTime;
// };

/**
 * Wait task node.
 * Wait for the specified time when executed.
 */
//UCLASS()
class UBTTask_Wait : public UBTTaskNode
{
public:
	UBTTask_Wait();

	mutable float RemainingWaitTime;

	/** wait time in seconds */
	//UPROPERTY(Category = Wait, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float WaitTime;

	/** allows adding random time to wait time */
	//UPROPERTY(Category = Wait, EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
	float RandomDeviation;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual uint16 GetInstanceMemorySize() const;
	virtual FString GetDescription() const;
// 	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const;
// 	virtual FString GetStaticDescription() const;
//
// #if WITH_EDITOR
// 	virtual FName GetNodeIconName() const;
// #endif // WITH_EDITOR

protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);


//////////////////////////////////////////////////////////////////////////
// BnS Begin: ID(ServerBehavior) modifier:(jarlmeng)
// #if WITH_EDITOR
// public:
// 	virtual class FXmlNode* ExportXml(class FXmlNode* ParentNode, class FXmlNode* TypesNode, TSet<UBehaviorTree*>& SubTrees) const;
// #endif
// BnS End
//////////////////////////////////////////////////////////////////////////
};
