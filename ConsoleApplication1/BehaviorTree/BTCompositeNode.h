// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once
//
// #include "CoreMinimal.h"
// #include "UObject/ObjectMacros.h"
#include "BehaviorTree/BTNode.h"
//#include "BTCompositeNode.generated.h"

class UBTCompositeNode;
class UBTDecorator;
class UBTService;
class UBTTaskNode;

// DECLARE_DELEGATE_RetVal_ThreeParams(int32, FGetNextChildDelegate, FBehaviorTreeSearchData& /*search data*/, int32 /*last child index*/, EBTNodeResult::Type /*last result*/);
//
// struct FBTCompositeMemory
// {
// 	/** index of currently active child node */
// 	int8 CurrentChild;
//
// 	/** child for next selection */
// 	int8Child;
// };

enum EBTChildIndex
{
	EBTChildIndex_FirstNode,
	EBTChildIndex_TaskNode,
};

namespace EBTDecoratorLogic
{
// keep in sync with DescribeLogicOp() in BTCompositeNode.cpp

enum Type
{
	Invalid,
	/** Test decorator conditions. */
	Test,
	/** logic op: AND */
	And,
	/** logic op: OR */
	Or,
	/** logic op: NOT */
	Not,
};
}

struct FBTDecoratorLogic
{
	EBTDecoratorLogic::Type Operation;

	uint16 Number;

	FBTDecoratorLogic() : Operation(EBTDecoratorLogic::Invalid), Number(0) {}
	FBTDecoratorLogic(uint8 InOperation, uint16 InNumber) : Operation((EBTDecoratorLogic::Type)InOperation), Number(InNumber) {}
};

struct FBTCompositeChild
{
	UBTCompositeNode* ChildComposite;
	UBTTaskNode* ChildTask;
	std::vector<UBTDecorator*> Decorators;
	std::vector<FBTDecoratorLogic> DecoratorOps;
};

class UBTCompositeNode : public UBTNode
{
public:
	UBTCompositeNode();
	static uint32 sNodeTypeFlag;
	friend class UBehaviorTreeManager;

	std::vector<FBTCompositeChild> Children;

	/** index of currently active child node */
	mutable int8 CurrentChild;

	/** child for next selection */
	mutable int8 OverrideChild;
	
//
// 	/** delegate for finding next child to execute */
// 	FGetNextChildDelegate OnNextChild;
	bool NeedCallGetNextChildInner;

	/** fill in data about tree structure */
	void InitializeComposite(uint16 InLastExecutionIndex);

	/** find next child branch to execute */
	int32 FindChildToExecute(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& LastResult) const;

	/** get index of child node (handle subtrees) */
	int32 GetChildIndex(FBehaviorTreeSearchData& SearchData, const UBTNode& ChildNode) const;
	/** get index of child node */
	int32 GetChildIndex(const UBTNode& ChildNode) const;

	/** called before passing search to child node */
	void OnChildActivation(FBehaviorTreeSearchData& SearchData, const UBTNode& ChildNode) const;
	void OnChildActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIndex) const;

	/** called after child has finished search */
	void OnChildDeactivation(FBehaviorTreeSearchData& SearchData, const UBTNode& ChildNode, EBTNodeResult::Type& NodeResult) const;
	void OnChildDeactivation(FBehaviorTreeSearchData& SearchData, int32 ChildIndex, EBTNodeResult::Type& NodeResult) const;

	/** called when start enters this node */
	void OnNodeActivation(FBehaviorTreeSearchData& SearchData) const;

	/** called when search leaves this node */
	void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const;

	/** called when search needs to reactivate this node */
	void OnNodeRestart(FBehaviorTreeSearchData& SearchData) const;

	/** notify about task execution start */
	void ConditionalNotifyChildExecution(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const UBTNode& ChildNode, EBTNodeResult::Type& NodeResult) const;

	/** size of instance memory */
	virtual uint16 GetInstanceMemorySize() const;

	/** @return child node at given index */
	UBTNode* GetChildNode(int32 Index) const
	{
		if (IsValidIndex(Children, Index))
		{
			return Children[Index].ChildComposite ?
			       (UBTNode*)Children[Index].ChildComposite :
			       (UBTNode*)Children[Index].ChildTask;
		}
		return NULL;
	}

	/** @return children count */
	int32 GetChildrenNum() const
	{
		return (int32)Children.size();
	}

	/** @return execution index of child node */
	uint16 GetChildExecutionIndex(int32 Index, EBTChildIndex ChildMode = EBTChildIndex_TaskNode) const;

	/** @return execution index of last node in child branches */
	uint16 GetLastExecutionIndex() const
	{
		return LastExecutionIndex;
	}

	/** set for next child index */
	virtual void SetChildOverride(FBehaviorTreeSearchData& SearchData, int8 Index) const;

	/** gathers description of all runtime parameters */
	//virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, std::vector<FString>& Values) const;

	/** check if child node can execute new subtree */
	virtual bool CanPushSubtree(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, int32 ChildIdx) const;

// #if WITH_EDITOR
// 	/** @return allowed flow abort modes for decorators */
// 	virtual bool CanAbortLowerPriority() const;
// 	virtual bool CanAbortSelf() const;
// #endif // WITH_EDITOR

	/** find branch containing specified node index */
	int32 GetMatchingChildIndex(int32 ActiveInstanceIdx, FBTNodeIndex& NodeIdx) const;

	/** get first execution index of given branch */
	uint16 GetBranchExecutionIndex(uint16 NodeInBranchIdx) const;

	/** is child execution allowed by decorators? */
	bool DoDecoratorsAllowExecution(UBehaviorTreeComponent& OwnerComp, int32 InstanceIdx, int32 ChildIdx) const;

	bool IsApplyingDecoratorScope() const
	{
		return bApplyDecoratorScope;
	}

public:

	/** if set, all decorators in branch below will be removed when execution flow leaves (decorators on this node are not affected) */
	uint32 bApplyDecoratorScope : 1;

	/** if set, NotifyChildExecution will be called */
	uint32 bUseChildExecutionNotify : 1;

	/** if set, NotifyNodeActivation will be called */
	uint32 bUseNodeActivationNotify : 1;

	/** if set, NotifyNodeDeactivation will be called */
	uint32 bUseNodeDeactivationNotify : 1;

	/** if set, CanNotifyDecoratorsOnActivation will be called */
	uint32 bUseDecoratorsActivationCheck : 1;

	/** if set, CanNotifyDecoratorsOnDeactivation will be called */
	uint32 bUseDecoratorsDeactivationCheck : 1;

	/** if set, CanNotifyDecoratorsOnFailedActivation will be called */
	uint32 bUseDecoratorsFailedActivationCheck : 1;

	/** execution index of last node in child branches */
	uint16 LastExecutionIndex;

	/** called just after child execution, allows to modify result */
	virtual void NotifyChildExecution(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const;

	/** called when start enters this node */
	virtual void NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const;

	/** called when start leaves this node */
	virtual void NotifyNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const;

	/** check if NotifyDecoratorsOnActivation is allowed, requires bUseDecoratorsActivationCheck flag  */
	virtual bool CanNotifyDecoratorsOnActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx) const;

	/** check if NotifyDecoratorsOnDeactivation is allowed, requires bUseDecoratorsDeactivationCheck flag  */
	virtual bool CanNotifyDecoratorsOnDeactivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const;

	/** check if NotifyDecoratorsOnFailedActivation is allowed, requires bUseDecoratorsActivationCheck flag  */
	virtual bool CanNotifyDecoratorsOnFailedActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const;

	/** runs through decorators on given child node and notify them about activation */
	void NotifyDecoratorsOnActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx) const;

	/** runs through decorators on given child node and notify them about deactivation */
	void NotifyDecoratorsOnDeactivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const;

	/** runs through decorators on given child node and notify them about failed activation */
	void NotifyDecoratorsOnFailedActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const;

	/** get next child to process and store it in CurrentChild */
	int32 GetNextChild(FBehaviorTreeSearchData& SearchData, int32 LastChildIdx, EBTNodeResult::Type LastResult) const;

	virtual int32 _GetNextChildInner(FBehaviorTreeSearchData& SearchData, int32 LastChildIdx, EBTNodeResult::Type LastResult) const;

	/** store delayed execution request */
	void RequestDelayedExecution(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type LastResult) const;

//////////////////////////////////////////////////////////////////////////
// BnS Begin: ID(ServerBehavior) modifier:(jarlmeng)
// #if WITH_EDITOR
// public:
// 	virtual FString GetXmlTagName() const { return TEXT("Composite"); }
// 	virtual class FXmlNode* ExportXml(class FXmlNode* ParentNode, class FXmlNode* TypesNode, TSet<UBehaviorTree*>& SubTrees) const;
// 	virtual void ExportXml_Decorators(class FXmlNode* ParentNode, class FXmlNode* TypesNode, const std::vector<UBTDecorator*>& Decorators, const std::vector<FBTDecoratorLogic>& DecoratorOps, TSet<UBehaviorTree*>& SubTrees) const;
// #endif
	// BnS End
//////////////////////////////////////////////////////////////////////////
};

