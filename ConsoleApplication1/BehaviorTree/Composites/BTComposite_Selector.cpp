// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTComposite_Selector, UBTComposite_Selector_Factory, CompositeNodeFactoryBase)
END_DERIVED_NODE_FACTORY(UBTComposite_Selector, UBTComposite_Selector_Factory)

UBTComposite_Selector::UBTComposite_Selector() : UBTCompositeNode()
{
	NodeName = "Selector";

	//OnNextChild.BindUObject(this, &UBTComposite_Selector::GetNextChildHandler);
}

int32 UBTComposite_Selector::_GetNextChildInner(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	// success = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		// newly activated: start from first
		NextChildIdx = 0;
	}
	else if (LastResult == EBTNodeResult::Failed && (PrevChild + 1) < GetChildrenNum())
	{
		// failed = choose next child
		NextChildIdx = PrevChild + 1;
	}


	//UE_VLOG("UBTComposite_Selector::_GetNextChildInner NextChildIdx:%d\n", NextChildIdx);

	return NextChildIdx;
}

