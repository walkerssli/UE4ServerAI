// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/Composites/BTComposite_Sequence.h"
#include "BehaviorTree/BehaviorTreeParseHelper.h"

BEGIN_DERIVED_NODE_FACTORY(UBTComposite_Sequence, UBTComposite_Sequence_Factory, CompositeNodeFactoryBase)
END_DERIVED_NODE_FACTORY(UBTComposite_Sequence, UBTComposite_Sequence_Factory)

UBTComposite_Sequence::UBTComposite_Sequence() : UBTCompositeNode()
{
	NodeName = "Sequence";

	//OnNextChild.BindUObject(this, &UBTComposite_Sequence::GetNextChildHandler);
}

int32 UBTComposite_Sequence::_GetNextChildInner(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	// failure = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		// newly activated: start from first
		NextChildIdx = 0;
	}
	else if (LastResult == EBTNodeResult::Succeeded && (PrevChild + 1) < GetChildrenNum())
	{
		// success = choose next child
		NextChildIdx = PrevChild + 1;
	}

	//UE_VLOG("UBTComposite_Sequence::_GetNextChildInner NextChildIdx:%d\n", NextChildIdx);
	return NextChildIdx;
}


