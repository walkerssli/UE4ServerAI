// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

//#include "CoreMinimal.h"
//#include "UObject/ObjectMacros.h"
#include "BehaviorTree/BTCompositeNode.h"
//#include "BTComposite_Selector.generated.h"

/** 
 * Selector composite node.
 * Selector Nodes execute their children from left to right, and will stop executing its children when one of their children succeeds.
 * If a Selector's child succeeds, the Selector succeeds. If all the Selector's children fail, the Selector fails.
 */
//UCLASS()
class UBTComposite_Selector: public UBTCompositeNode
{
public:
	UBTComposite_Selector();

	virtual int32 _GetNextChildInner(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;

};
