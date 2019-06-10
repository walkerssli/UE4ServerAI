// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

// #include "CoreMinimal.h"
// #include "UObject/ObjectMacros.h"
#include "BehaviorTree/BTAuxiliaryNode.h"
//#include "BTDecorator.generated.h"

class FBehaviorDecoratorDetails;

enum EBTDecoratorAbortRequest
{
	// request execution update when only result of condition changes and active branch of tree can potentially change too
	ConditionResultChanged,

	// request execution update every time as long as condition is still passing
	ConditionPassing,
};

/**
 * Decorators are supporting nodes placed on parent-child connection, that receive notification about execution flow and can be ticked
 *
 * Because some of them can be instanced for specific AI, following virtual functions are not marked as const:
 *  - OnNodeActivation
 *  - OnNodeDeactivation
 *  - OnNodeProcessed
 *  - OnBecomeRelevant (from UBTAuxiliaryNode)
 *  - OnCeaseRelevant (from UBTAuxiliaryNode)
 *  - TickNode (from UBTAuxiliaryNode)
 *
 * If your node is not being instanced (default behavior), DO NOT change any properties of object within those functions!
 * Template nodes are shared across all behavior tree components using the same tree asset and must store
 * their runtime properties in provided NodeMemory block (allocation size determined by GetInstanceMemorySize() )
 *
 */

class UBTDecorator : public UBTAuxiliaryNode
{
public:
	UBTDecorator();
	static uint32 sNodeTypeFlag;

	/** wrapper for node instancing: CalculateRawConditionValue */
	bool WrappedCanExecute(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	/** wrapper for node instancing: OnNodeActivation  */
	void WrappedOnNodeActivation(FBehaviorTreeSearchData& SearchData) const;

	/** wrapper for node instancing: OnNodeDeactivation */
	void WrappedOnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) const;

	/** wrapper for node instancing: OnNodeProcessed */
	void WrappedOnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const;

	/** @return flow controller's abort mode */
	inline EBTFlowAbortMode::Type GetFlowAbortMode() const
	{
		return FlowAbortMode;
	}

	/** @return true if condition should be inversed */
	inline bool IsInversed() const
	{
		return bInverseCondition;
	}

	//virtual FString GetStaticDescription() const;

	/** modify current flow abort mode, so it can be used with parent composite */
	void UpdateFlowAbortMode();

	/** @return true if current abort mode can be used with parent composite */
	bool IsFlowAbortModeValid() const;

public:

	/** if set, FlowAbortMode can be set to None */
	uint32 bAllowAbortNone : 1;

	/** if set, FlowAbortMode can be set to LowerPriority and Both */
	uint32 bAllowAbortLowerPri : 1;

	/** if set, FlowAbortMode can be set to Self and Both */
	uint32 bAllowAbortChildNodes : 1;

	/** if set, OnNodeActivation will be used */
	uint32 bNotifyActivation : 1;

	/** if set, OnNodeDeactivation will be used */
	uint32 bNotifyDeactivation : 1;

	/** if set, OnNodeProcessed will be used */
	uint32 bNotifyProcessed : 1;

	/** if set, static description will include default description of inversed condition */
	uint32 bShowInverseConditionDesc : 1;

public:
	/** if set, condition check result will be inversed */
	uint32 bInverseCondition : 1;

public:
	/** flow controller settings */
	EBTFlowAbortMode::Type FlowAbortMode;

	void SetIsInversed(bool bShouldBeInversed);

	/** called when underlying node is activated
	  * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData);

	/** called when underlying node has finished
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult);

	/** called when underlying node was processed (deactivated or failed to activate)
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult);

	/** calculates raw, core value of decorator's condition. Should not include calling IsInversed */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	/** more "flow aware" version of calling RequestExecution(this) on owning behavior tree component
	 *  should be used in external events that may change result of CalculateRawConditionValue
	 */
	void ConditionalFlowAbort(UBehaviorTreeComponent& OwnerComp, EBTDecoratorAbortRequest RequestMode) const;

	friend class FBehaviorDecoratorDetails;

	//----------------------------------------------------------------------//
	// DEPRECATED
	//----------------------------------------------------------------------//
	//DEPRECATED(4.12, "This function is deprecated, please use InitializeParentLink instead.")
	void InitializeDecorator(uint8 InChildIndex);

//////////////////////////////////////////////////////////////////////////
// BnS Begin: ID(ServerBehavior) modifier:(jarlmeng)
// #if WITH_EDITOR
// public:
// 	virtual FString GetXmlTagName() const { return TEXT("Decorator"); }
// 	virtual class FXmlNode* ExportXml(class FXmlNode* ParentNode, class FXmlNode* TypesNode, TSet<UBehaviorTree*>& SubTrees) const;
// #endif
// BnS End
//////////////////////////////////////////////////////////////////////////
};


//////////////////////////////////////////////////////////////////////////
// Inlines
