// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once
// 
// #include "CoreMinimal.h"
// #include "UObject/ObjectMacros.h"
// #include "UObject/Object.h"
// #include "Engine/Blueprint.h"
#include "BehaviorTree/BTCompositeNode.h"
//#include "BehaviorTree.generated.h"

class UBlackboardData;
class UBTDecorator;

class UBehaviorTree
{
public:
	UBehaviorTree(const FString& name, int nodeMemory);
	
	~UBehaviorTree();

	/** root node of loaded tree */
	UBTCompositeNode* RootNode;

	char* GetNodeMemoryPtr(int sz);
	inline int GetNodeMemorySize() const { return mNodeMemorySize; }

	/** root level decorators, used by subtrees */
	std::vector<UBTDecorator*> RootDecorators;

	/** logic operators for root level decorators, used by subtrees  */
	std::vector<FBTDecoratorLogic> RootDecoratorOps;

	/** memory size required for instance of this tree */
	uint16 InstanceMemorySize;

	FString mName;

	FString mBlackboardName;

	inline const FString& GetName() const { return mName; }

	inline const FString& GetBBName() const { return mBlackboardName; }
protected:
	char* mNodeMemory;
	int mNodeMemorySize;
	int mUsedMemorySize;
};
