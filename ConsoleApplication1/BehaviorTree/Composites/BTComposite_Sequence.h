// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "BehaviorTree/BTCompositeNode.h"

/**
 * Sequence composite node.
 * Sequence Nodes execute their children from left to right, and will stop executing its children when one of their children fails.
 * If a child fails, then the Sequence fails. If all the Sequence's children succeed, then the Sequence succeeds.
 */
//UCLASS()
class  UBTComposite_Sequence : public UBTCompositeNode
{
public:
	UBTComposite_Sequence();

	virtual int32 _GetNextChildInner(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;

};
