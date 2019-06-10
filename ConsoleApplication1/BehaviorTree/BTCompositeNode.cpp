// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/BTCompositeNode.h"
// #include "GameFramework/Actor.h"
// #include "VisualLogger/VisualLogger.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BTService.h"

UBTCompositeNode::UBTCompositeNode() : UBTNode()
{
	NodeType |= (uint32)BTNodeTypes_CompositeNode;
	NodeName = "UnknownComposite";
	bApplyDecoratorScope = false;
	bUseChildExecutionNotify = false;
	bUseNodeActivationNotify = false;
	bUseNodeDeactivationNotify = false;
	bUseDecoratorsActivationCheck = false;
	bUseDecoratorsDeactivationCheck = false;
	bUseDecoratorsFailedActivationCheck = false;
	NeedCallGetNextChildInner = true;
}


uint32 UBTCompositeNode::sNodeTypeFlag = (uint32)BTNodeTypes_CompositeNode;

void UBTCompositeNode::InitializeComposite(uint16 InLastExecutionIndex)
{
	LastExecutionIndex = InLastExecutionIndex;
}

int32 UBTCompositeNode::FindChildToExecute(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& LastResult) const
{
	int32 RetIdx = BTSpecialChild::ReturnToParent;

	if (Children.size())
	{
		int32 ChildIdx = GetNextChild(SearchData, CurrentChild, LastResult);
		while (IsValidIndex(Children, ChildIdx) && !SearchData.bPostponeSearch)
		{
			// check decorators
			if (DoDecoratorsAllowExecution(SearchData.OwnerComp, SearchData.OwnerComp.ActiveInstanceIdx, ChildIdx))
			{
				OnChildActivation(SearchData, ChildIdx);
				RetIdx = ChildIdx;
				break;
			}
			else
			{
				LastResult = EBTNodeResult::Failed;

				const bool bCanNotify = !bUseDecoratorsFailedActivationCheck || CanNotifyDecoratorsOnFailedActivation(SearchData, ChildIdx, LastResult);
				if (bCanNotify)
				{
					NotifyDecoratorsOnFailedActivation(SearchData, ChildIdx, LastResult);
				}
			}

			ChildIdx = GetNextChild(SearchData, ChildIdx, LastResult);
		}
	}

	return RetIdx;
}

int32 UBTCompositeNode::GetChildIndex(FBehaviorTreeSearchData& SearchData, const UBTNode& ChildNode) const
{
	if (ChildNode.GetParentNode() != this)
	{
		return CurrentChild;
	}

	return GetChildIndex(ChildNode);
}

int32 UBTCompositeNode::GetChildIndex(const UBTNode& ChildNode) const
{
	for (int32 ChildIndex = 0; ChildIndex < (int)Children.size(); ChildIndex++)
	{
		if (Children[ChildIndex].ChildComposite == &ChildNode ||
		        Children[ChildIndex].ChildTask == &ChildNode)
		{
			return ChildIndex;
		}
	}

	return BTSpecialChild::ReturnToParent;
}

void UBTCompositeNode::OnChildActivation(FBehaviorTreeSearchData& SearchData, const UBTNode& ChildNode) const
{
	OnChildActivation(SearchData, GetChildIndex(SearchData, ChildNode));
}

void UBTCompositeNode::OnChildActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIndex) const
{
	const FBTCompositeChild& ChildInfo = Children[ChildIndex];

	// pass to decorators before changing current child in node memory
	// so they can access previously executed one if needed
	const bool bCanNotify = !bUseDecoratorsActivationCheck || CanNotifyDecoratorsOnActivation(SearchData, ChildIndex);
	if (bCanNotify)
	{
		NotifyDecoratorsOnActivation(SearchData, ChildIndex);
	}

	// don't activate task services here, it's applied BEFORE aborting (e.g. abort lower pri decorator)
	// use UBehaviorTreeComponent::ExecuteTask instead

	// pass to child composite
	if (ChildInfo.ChildComposite)
	{
		ChildInfo.ChildComposite->OnNodeActivation(SearchData);
	}

	// update active node in current context: child node
	CurrentChild = ChildIndex;
}

void UBTCompositeNode::OnChildDeactivation(FBehaviorTreeSearchData& SearchData, const UBTNode& ChildNode, EBTNodeResult::Type& NodeResult) const
{
	OnChildDeactivation(SearchData, GetChildIndex(SearchData, ChildNode), NodeResult);
}

void UBTCompositeNode::OnChildDeactivation(FBehaviorTreeSearchData& SearchData, int32 ChildIndex, EBTNodeResult::Type& NodeResult) const
{
	if (ChildIndex < 0 || ChildIndex >= (int)Children.size())
	{
		return;
	}
	const FBTCompositeChild& ChildInfo = Children[ChildIndex];

	// pass to task services
	if (ChildInfo.ChildTask)
	{
		for (int32 ServiceIndex = 0; ServiceIndex < (int)ChildInfo.ChildTask->Services.size(); ServiceIndex++)
		{
			SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(ChildInfo.ChildTask->Services[ServiceIndex], SearchData.OwnerComp.GetActiveInstanceIdx(), EBTNodeUpdateMode::Remove));
		}
	}
	// pass to child composite
	else if (ChildInfo.ChildComposite)
	{
		ChildInfo.ChildComposite->OnNodeDeactivation(SearchData, NodeResult);
	}

	// pass to decorators after composite is updated (so far only simple parallel uses it)
	// to have them working on correct result + they must be able to modify it if requested (e.g. force success)
	const bool bCanNotify = !bUseDecoratorsDeactivationCheck || CanNotifyDecoratorsOnDeactivation(SearchData, ChildIndex, NodeResult);
	if (bCanNotify)
	{
		NotifyDecoratorsOnDeactivation(SearchData, ChildIndex, NodeResult);
	}
}

void UBTCompositeNode::OnNodeActivation(FBehaviorTreeSearchData& SearchData) const
{
	OnNodeRestart(SearchData);

	if (bUseNodeActivationNotify)
	{
		NotifyNodeActivation(SearchData);
	}

	for (int32 ServiceIndex = 0; ServiceIndex < (int)Services.size(); ServiceIndex++)
	{
		// add services when execution flow enters this composite
		SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(Services[ServiceIndex], SearchData.OwnerComp.GetActiveInstanceIdx(), EBTNodeUpdateMode::Add));

		// give services chance to perform initial tick before searching further
		Services[ServiceIndex]->NotifyParentActivation(SearchData);
	}
}

void UBTCompositeNode::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const
{
	if (bUseNodeDeactivationNotify)
	{
		NotifyNodeDeactivation(SearchData, NodeResult);
	}

	// remove all services if execution flow leaves this composite
	for (int32 ServiceIndex = 0; ServiceIndex < (int)Services.size(); ServiceIndex++)
	{
		SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(Services[ServiceIndex], SearchData.OwnerComp.GetActiveInstanceIdx(), EBTNodeUpdateMode::Remove));
	}

	// optional: remove all decorators if execution flow leaves this composite
	if (bApplyDecoratorScope)
	{
		const uint16 InstanceIdx = SearchData.OwnerComp.GetActiveInstanceIdx();
		const FBTNodeIndex FromIndex(InstanceIdx, GetExecutionIndex());
		const FBTNodeIndex ToIndex(InstanceIdx, GetLastExecutionIndex());

		SearchData.OwnerComp.UnregisterAuxNodesInRange(FromIndex, ToIndex);

		// remove all pending updates "Add"
		for (int32 Idx = SearchData.PendingUpdates.size() - 1; Idx >= 0; Idx--)
		{
			const FBehaviorTreeSearchUpdate& UpdateInfo = SearchData.PendingUpdates[Idx];
			if (UpdateInfo.Mode == EBTNodeUpdateMode::Add)
			{
				const uint16 UpdateNodeIdx = UpdateInfo.AuxNode ? UpdateInfo.AuxNode->GetExecutionIndex() : UpdateInfo.TaskNode->GetExecutionIndex();
				const FBTNodeIndex UpdateIdx(UpdateInfo.InstanceIndex, UpdateNodeIdx);

				if (FromIndex.TakesPriorityOver(UpdateIdx) && UpdateIdx.TakesPriorityOver(ToIndex))
				{
// 					UE_VLOG(SearchData.OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("Search node update[canceled]: %s"),
// 						*UBehaviorTreeTypes::DescribeNodeHelper(UpdateInfo.AuxNode ? (UBTNode*)UpdateInfo.AuxNode : (UBTNode*)UpdateInfo.TaskNode));

					SearchData.PendingUpdates.erase(SearchData.PendingUpdates.begin() + Idx);
				}
			}
		}
	}
}

void UBTCompositeNode::OnNodeRestart(FBehaviorTreeSearchData& SearchData) const
{
	CurrentChild = BTSpecialChild::NotInitialized;
	OverrideChild = BTSpecialChild::NotInitialized;
}

void UBTCompositeNode::NotifyDecoratorsOnActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx) const
{
	const FBTCompositeChild& ChildInfo = Children[ChildIdx];
	for (int32 DecoratorIndex = 0; DecoratorIndex < (int)ChildInfo.Decorators.size(); DecoratorIndex++)
	{
		const UBTDecorator* DecoratorOb = ChildInfo.Decorators[DecoratorIndex];
		DecoratorOb->WrappedOnNodeActivation(SearchData);

		switch (DecoratorOb->GetFlowAbortMode())
		{
		case EBTFlowAbortMode::LowerPriority:
			SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(DecoratorOb, SearchData.OwnerComp.GetActiveInstanceIdx(), EBTNodeUpdateMode::Remove));
			break;

		case EBTFlowAbortMode::Self:
		case EBTFlowAbortMode::Both:
			SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(DecoratorOb, SearchData.OwnerComp.GetActiveInstanceIdx(), EBTNodeUpdateMode::Add));
			break;

		default:
			break;
		}
	}
}

void UBTCompositeNode::NotifyDecoratorsOnDeactivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const
{
	const FBTCompositeChild& ChildInfo = Children[ChildIdx];
	if (NodeResult == EBTNodeResult::Aborted)
	{
		// simple notify when aborting execution:
		// - search update will be collected separately (UBehaviorTreeComponent::UnregisterAuxNodesUpTo)
		// - can't modify result in OnNodeProcessed
		for (int32 DecoratorIndex = 0; DecoratorIndex < (int)ChildInfo.Decorators.size(); DecoratorIndex++)
		{
			const UBTDecorator* DecoratorOb = ChildInfo.Decorators[DecoratorIndex];
			DecoratorOb->WrappedOnNodeDeactivation(SearchData, NodeResult);
		}
	}
	else
	{
		// regular execution flow
		for (int32 DecoratorIndex = 0; DecoratorIndex < (int)ChildInfo.Decorators.size(); DecoratorIndex++)
		{
			const UBTDecorator* DecoratorOb = ChildInfo.Decorators[DecoratorIndex];
			DecoratorOb->WrappedOnNodeProcessed(SearchData, NodeResult);
			DecoratorOb->WrappedOnNodeDeactivation(SearchData, NodeResult);

			// leaving child branch:
			if (DecoratorOb->GetFlowAbortMode() == EBTFlowAbortMode::Self)
			{
				// - observers with mode "Self" are now out of scope, remove them
				SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(DecoratorOb, SearchData.OwnerComp.GetActiveInstanceIdx(), EBTNodeUpdateMode::Remove));
			}
			else if (DecoratorOb->GetFlowAbortMode() == EBTFlowAbortMode::LowerPriority)
			{
				// - observers with mode "Lower Priority" will try to reactivate themselves ("Both" is not removed on node activation)
				SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(DecoratorOb, SearchData.OwnerComp.GetActiveInstanceIdx(), EBTNodeUpdateMode::Add));
			}
		}
	}
}

void UBTCompositeNode::NotifyDecoratorsOnFailedActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const
{
	const FBTCompositeChild& ChildInfo = Children[ChildIdx];
	const uint16 ActiveInstanceIdx = SearchData.OwnerComp.GetActiveInstanceIdx();

	for (int32 DecoratorIndex = 0; DecoratorIndex < (int)ChildInfo.Decorators.size(); DecoratorIndex++)
	{
		const UBTDecorator* DecoratorOb = ChildInfo.Decorators[DecoratorIndex];
		DecoratorOb->WrappedOnNodeProcessed(SearchData, NodeResult);

		if (DecoratorOb->GetFlowAbortMode() == EBTFlowAbortMode::LowerPriority ||
		        DecoratorOb->GetFlowAbortMode() == EBTFlowAbortMode::Both)
		{
			SearchData.AddUniqueUpdate(FBehaviorTreeSearchUpdate(DecoratorOb, ActiveInstanceIdx, EBTNodeUpdateMode::Add));
		}
	}

	//SearchData.OwnerComp.StoreDebuggerSearchStep(GetChildNode(ChildIdx), ActiveInstanceIdx, NodeResult);
}

void UBTCompositeNode::NotifyChildExecution(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const
{
}

void UBTCompositeNode::NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const
{
}

void UBTCompositeNode::NotifyNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const
{
}

void UBTCompositeNode::ConditionalNotifyChildExecution(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const UBTNode& ChildNode, EBTNodeResult::Type& NodeResult) const
{
	if (bUseChildExecutionNotify)
	{
		for (int32 ChildIndex = 0; ChildIndex < (int)Children.size(); ChildIndex++)
		{
			if (Children[ChildIndex].ChildComposite == &ChildNode || Children[ChildIndex].ChildTask == &ChildNode)
			{
				NotifyChildExecution(OwnerComp, NodeMemory, ChildIndex, NodeResult);
				break;
			}
		}
	}
}

static bool IsLogicOp(const FBTDecoratorLogic& Info)
{
	return (Info.Operation != EBTDecoratorLogic::Test) && (Info.Operation != EBTDecoratorLogic::Invalid);
}

// static FString DescribeLogicOp(const TEnumAsByte<EBTDecoratorLogic::Type>& Op)
// {
// 	static FString LogicDesc[] = { TEXT("Invalid"), TEXT("Test"), TEXT("AND"), TEXT("OR"), TEXT("NOT") };
// 	return LogicDesc[Op];
// }

struct FOperationStackInfo
{
	uint16 NumLeft;
	EBTDecoratorLogic::Type Op;
	uint8 bHasForcedResult : 1;
	uint8 bForcedResult : 1;

	FOperationStackInfo() {}
	FOperationStackInfo(const FBTDecoratorLogic& DecoratorOp) :
		NumLeft(DecoratorOp.Number), Op(DecoratorOp.Operation), bHasForcedResult(0) {};
};

static bool UpdateOperationStack(const UBehaviorTreeComponent& OwnerComp, FString& Indent,
                                 std::vector<FOperationStackInfo>& Stack, bool bTestResult,
                                 int32& FailedDecoratorIdx, int32& NodeDecoratorIdx, bool& bShouldStoreNodeIndex)
{
	size_t n = Stack.size();
	if (n == 0)
	{
		return bTestResult;
	}

	FOperationStackInfo& CurrentOp = Stack[n - 1];
	CurrentOp.NumLeft--;

	if (CurrentOp.Op == EBTDecoratorLogic::And)
	{
		if (!CurrentOp.bHasForcedResult && !bTestResult)
		{
			CurrentOp.bHasForcedResult = true;
			CurrentOp.bForcedResult = bTestResult;
		}
	}
	else if (CurrentOp.Op == EBTDecoratorLogic::Or)
	{
		if (!CurrentOp.bHasForcedResult && bTestResult)
		{
			CurrentOp.bHasForcedResult = true;
			CurrentOp.bForcedResult = bTestResult;
		}
	}
	else if (CurrentOp.Op == EBTDecoratorLogic::Not)
	{
		bTestResult = !bTestResult;
	}

	// update debugger while processing top level stack
	if (n == 1)
	{
		// reset node flag and grab next decorator index
		bShouldStoreNodeIndex = true;

		// store first failed node
		if (!bTestResult && FailedDecoratorIdx == INDEX_NONE)
		{
			FailedDecoratorIdx = NodeDecoratorIdx;
		}
	}

	if (CurrentOp.bHasForcedResult)
	{
		bTestResult = CurrentOp.bForcedResult;
	}

	if (CurrentOp.NumLeft == 0)
	{
// 		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("%s%s finished: %s"), *Indent,
// 			*DescribeLogicOp(CurrentOp.Op),
// 			bTestResult ? TEXT("allowed") : TEXT("forbidden"));
// 		Indent = Indent.LeftChop(2);

		Stack.erase(Stack.begin() + n - 1);
		return UpdateOperationStack(OwnerComp, Indent, Stack, bTestResult, FailedDecoratorIdx, NodeDecoratorIdx, bShouldStoreNodeIndex);
	}

	return bTestResult;
}

bool UBTCompositeNode::DoDecoratorsAllowExecution(UBehaviorTreeComponent& OwnerComp, int32 InstanceIdx, int32 ChildIdx) const
{
	assert(IsValidIndex(Children, ChildIdx));
	if (!IsValidIndex(Children, ChildIdx))
	{
		return false;
	}

	const FBTCompositeChild& ChildInfo = Children[ChildIdx];
	bool bResult = true;

	if (ChildInfo.Decorators.size() == 0)
	{
		return bResult;
	}

	FBehaviorTreeInstance& MyInstance = OwnerComp.InstanceStack[InstanceIdx];

	if (ChildInfo.DecoratorOps.size() == 0)
	{
		// simple check: all decorators must agree
		for (int32 DecoratorIndex = 0; DecoratorIndex < (int)ChildInfo.Decorators.size(); DecoratorIndex++)
		{
			const UBTDecorator* TestDecorator = ChildInfo.Decorators[DecoratorIndex];
			const bool bIsAllowed = TestDecorator ? TestDecorator->WrappedCanExecute(OwnerComp, TestDecorator->GetNodeMemory<uint8>(MyInstance)) : false;
			//OwnerComp.StoreDebuggerSearchStep(TestDecorator, InstanceIdx, bIsAllowed);

			if (!bIsAllowed)
			{
// 				UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("Child[%d] execution forbidden by %s"),
// 					ChildIdx, *UBehaviorTreeTypes::DescribeNodeHelper(TestDecorator));

				bResult = false;
				break;
			}
			else
			{
// 				UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("Child[%d] execution allowed by %s"),
// 					ChildIdx, *UBehaviorTreeTypes::DescribeNodeHelper(TestDecorator));
			}
		}
	}
	else
	{
		// advanced check: follow decorator logic operations (composite decorator on child link)
//		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("Child[%d] execution test with logic operations"), ChildIdx);

		std::vector<FOperationStackInfo> OperationStack;
		FString Indent;

		// debugger data collection:
		// - get index of each decorator from main AND test, they will match graph nodes
		// - if first operator is not AND it means, that there's only single composite decorator on line
		// - while updating top level stack, grab index of first failed node

		int32 NodeDecoratorIdx = INDEX_NONE;
		int32 FailedDecoratorIdx = INDEX_NONE;
		bool bShouldStoreNodeIndex = true;

		for (int32 OperationIndex = 0; OperationIndex < (int)ChildInfo.DecoratorOps.size(); OperationIndex++)
		{
			const FBTDecoratorLogic& DecoratorOp = ChildInfo.DecoratorOps[OperationIndex];
			if (IsLogicOp(DecoratorOp))
			{
				OperationStack.push_back(FOperationStackInfo(DecoratorOp));
// 				Indent += TEXT("  ");
// 				UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("%spushed %s:%d"), *Indent,
// 					*DescribeLogicOp(DecoratorOp.Operation), DecoratorOp.Number);
			}
			else if (DecoratorOp.Operation == EBTDecoratorLogic::Test)
			{
				const bool bHasOverride = OperationStack.size() ? OperationStack[OperationStack.size() - 1].bHasForcedResult : false;
				const bool bCurrentOverride = OperationStack.size() ? OperationStack[OperationStack.size() - 1].bForcedResult : false;

				// debugger: store first decorator of graph node
				if (bShouldStoreNodeIndex)
				{
					bShouldStoreNodeIndex = false;
					NodeDecoratorIdx = DecoratorOp.Number;
				}

				UBTDecorator* TestDecorator = ChildInfo.Decorators[DecoratorOp.Number];
				const bool bIsAllowed = bHasOverride ? bCurrentOverride : TestDecorator->WrappedCanExecute(OwnerComp, TestDecorator->GetNodeMemory<uint8>(MyInstance));
// 				UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("%s%s %s: %s"), *Indent,
// 					bHasOverride ? TEXT("skipping") : TEXT("testing"),
// 					*UBehaviorTreeTypes::DescribeNodeHelper(TestDecorator),
// 					bIsAllowed ? TEXT("allowed") : TEXT("forbidden"));

				bResult = UpdateOperationStack(OwnerComp, Indent, OperationStack, bIsAllowed, FailedDecoratorIdx, NodeDecoratorIdx, bShouldStoreNodeIndex);
				if (OperationStack.size() == 0)
				{
// 					UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Verbose, TEXT("finished execution test: %s"),
// 						bResult ? TEXT("allowed") : TEXT("forbidden"));

					//OwnerComp.StoreDebuggerSearchStep(ChildInfo.Decorators[FMath::Max(0, FailedDecoratorIdx)], InstanceIdx, bResult);
					break;
				}
			}
		}
	}

	return bResult;
}

int32 UBTCompositeNode::GetMatchingChildIndex(int32 ActiveInstanceIdx, FBTNodeIndex& NodeIdx) const
{
	const int32 OutsideRange = BTSpecialChild::ReturnToParent;
	const int32 UnlimitedRange = (int)Children.size() - 1;

	// search ends at the same instance level: use execution index to determine branch containing node index
	if (ActiveInstanceIdx == NodeIdx.InstanceIndex)
	{
		// is composite even in range of search?
		if (GetExecutionIndex() > NodeIdx.ExecutionIndex)
		{
			return OutsideRange;
		}

		// find child outside range
		for (int32 ChildIndex = 0; ChildIndex < (int)Children.size(); ChildIndex++)
		{
			const uint16 FirstIndexInBranch = GetChildExecutionIndex(ChildIndex, EBTChildIndex_FirstNode);
			if (FirstIndexInBranch > NodeIdx.ExecutionIndex)
			{
				return ChildIndex ? (ChildIndex - 1) : 0;
			}
		}

		return UnlimitedRange;
	}

	// search ends at higher level: allow every node
	// search ends at lower level: outside allowed range
	return (ActiveInstanceIdx > NodeIdx.InstanceIndex) ? UnlimitedRange : OutsideRange;
}

uint16 UBTCompositeNode::GetBranchExecutionIndex(uint16 NodeInBranchIdx) const
{
	uint16 PrevBranchStartIdx = GetExecutionIndex();
	for (int32 ChildIndex = 0; ChildIndex < (int)Children.size(); ChildIndex++)
	{
		const uint16 BranchStartIdx = GetChildExecutionIndex(ChildIndex, EBTChildIndex_FirstNode);
		if (BranchStartIdx > NodeInBranchIdx)
		{
			break;
		}

		PrevBranchStartIdx = BranchStartIdx;
	}

	return PrevBranchStartIdx;
}

int32 UBTCompositeNode::GetNextChild(FBehaviorTreeSearchData& SearchData, int32 LastChildIdx, EBTNodeResult::Type LastResult) const
{
	int32 NextChildIndex = BTSpecialChild::ReturnToParent;
	uint16 ActiveInstanceIdx = SearchData.OwnerComp.GetActiveInstanceIdx();

	// newly activated node, search range not reached yet: select search branch for decorator test
	if (LastChildIdx == BTSpecialChild::NotInitialized && SearchData.SearchStart.IsSet() &&
	        FBTNodeIndex(ActiveInstanceIdx, GetExecutionIndex()).TakesPriorityOver(SearchData.SearchStart))
	{
		NextChildIndex = GetMatchingChildIndex(ActiveInstanceIdx, SearchData.SearchStart);
	}
	else if (OverrideChild != BTSpecialChild::NotInitialized && !SearchData.OwnerComp.IsRestartPending())
	{
		NextChildIndex = OverrideChild;
		OverrideChild = BTSpecialChild::NotInitialized;
	}
	// or use composite's logic
	else if (NeedCallGetNextChildInner)
	{
		NextChildIndex = _GetNextChildInner(SearchData, LastChildIdx, LastResult);
	}

	return NextChildIndex;
}


int32 UBTCompositeNode::_GetNextChildInner(FBehaviorTreeSearchData& SearchData, int32 LastChildIdx, EBTNodeResult::Type LastResult) const
{
	return -1;
}

void UBTCompositeNode::SetChildOverride(FBehaviorTreeSearchData& SearchData, int8 Index) const
{
	if (IsValidIndex(Children, Index) || Index == BTSpecialChild::ReturnToParent)
	{
		OverrideChild = Index;
	}
}

void UBTCompositeNode::RequestDelayedExecution(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type LastResult) const
{
	OwnerComp.RequestExecution(LastResult);
}

uint16 UBTCompositeNode::GetChildExecutionIndex(int32 Index, EBTChildIndex ChildMode) const
{
	const UBTNode* ChildNode = GetChildNode(Index);
	if (ChildNode)
	{
		const int32 Offset = (ChildMode == EBTChildIndex_FirstNode) ? Children[Index].Decorators.size() : 0;
		return ChildNode->GetExecutionIndex() - Offset;
	}

	return (LastExecutionIndex + 1);
}

bool UBTCompositeNode::CanPushSubtree(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, int32 ChildIdx) const
{
	return true;
}

bool UBTCompositeNode::CanNotifyDecoratorsOnActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx) const
{
	return true;
}

bool UBTCompositeNode::CanNotifyDecoratorsOnDeactivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const
{
	return true;
}

bool UBTCompositeNode::CanNotifyDecoratorsOnFailedActivation(FBehaviorTreeSearchData& SearchData, int32 ChildIdx, EBTNodeResult::Type& NodeResult) const
{
	return true;
}

// void UBTCompositeNode::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, std::vector<FString>& Values) const
// {
// 	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
//
// 	if (Verbosity == EBTDescriptionVerbosity::Detailed)
// 	{
// 		FBTCompositeMemory* MyMemory = (FBTCompositeMemory*)NodeMemory;
// 		Values.Add(FString::Printf(TEXT("current child: %d"), MyMemory->CurrentChild));
// 		Values.Add(FString::Printf(TEXT("override child: %d"), MyMemory->OverrideChild));
// 	}
// }
//
// #if WITH_EDITOR
//
// bool UBTCompositeNode::CanAbortSelf() const
// {
// 	return true;
// }
//
// bool UBTCompositeNode::CanAbortLowerPriority() const
// {
// 	return true;
// }
//
// #endif

uint16 UBTCompositeNode::GetInstanceMemorySize() const
{
	//return sizeof(FBTCompositeMemory);
	return 0;
}
//
// //////////////////////////////////////////////////////////////////////////
// // BnS Begin: ID(ServerBehavior) modifier:(jarlmeng)
// #if WITH_EDITOR
// #include "XmlNode.h"
// #include "BehaviorTreeXmlHelper.h"
// FXmlNode* UBTCompositeNode::ExportXml(class FXmlNode* ParentNode, class FXmlNode* TypesNode, TSet<UBehaviorTree*>& SubTrees) const
// {
// 	auto CompositeNode = Super::ExportXml(ParentNode, TypesNode, SubTrees);
// 	CompositeNode->AppendAttribute(TEXT("bApplyDecoratorScope"), BTXmlUtils::GetBoolString(bApplyDecoratorScope));
//
// 	auto ServicesNode = BTXmlUtils::NewXmlNode(CompositeNode, TEXT("Services"));
// 	auto ChildrenNode = BTXmlUtils::NewXmlNode(CompositeNode, TEXT("Children"));
//
// 	for (const UBTService* Service : Services)
// 	{
// 		Service->ExportXml(ServicesNode, TypesNode, SubTrees);
// 	}
//
// 	for (const FBTCompositeChild& Child : Children)
// 	{
// 		auto ChildNode = BTXmlUtils::NewXmlNode(ChildrenNode, TEXT("Child"));
//
// 		if (Child.ChildComposite)
// 		{
// 			Child.ChildComposite->ExportXml(ChildNode, TypesNode, SubTrees);
// 		}
// 		else if (Child.ChildTask)
// 		{
// 			Child.ChildTask->ExportXml(ChildNode, TypesNode, SubTrees);
// 		}
//
// 		ExportXml_Decorators(ChildNode, TypesNode, Child.Decorators, Child.DecoratorOps, SubTrees);
// 	}
//
// 	return CompositeNode;
// }
// //
// void UBTCompositeNode::ExportXml_Decorators(class FXmlNode* ParentNode, class FXmlNode* TypesNode, const std::vector<UBTDecorator*>& Decorators, const std::vector<FBTDecoratorLogic>& DecoratorOps, TSet<UBehaviorTree*>& SubTrees) const
// {
// 	auto DecoratorsNode = BTXmlUtils::NewXmlNode(ParentNode, TEXT("Decorators"));
// 	auto DecoratorOpsNode = BTXmlUtils::NewXmlNode(ParentNode, TEXT("DecoratorOps"));
//
// 	for (const UBTDecorator* Decorator : Decorators)
// 	{
// 		Decorator->ExportXml(DecoratorsNode, TypesNode, SubTrees);
// 	}
//
// 	if (DecoratorOps.size() > 0)
// 	{
// 		BTXmlUtils::ExportEnumXml(TEXT("EBTDecoratorLogic"), TypesNode);
//
// 		for (const FBTDecoratorLogic& DecoratorOp : DecoratorOps)
// 		{
// 			auto Node = BTXmlUtils::NewXmlNode(DecoratorOpsNode, TEXT("Logic"));
// 			Node->AppendAttribute(TEXT("Operation"), BTXmlUtils::GetEnumString(TEXT("EBTDecoratorLogic"), (int32)DecoratorOp.Operation));
// 			Node->AppendAttribute(TEXT("OperationValue"), FString::FromInt((int32)DecoratorOp.Operation));
// 			Node->AppendAttribute(TEXT("Number"), FString::FromInt(DecoratorOp.Number));
// 		}
// 	}
// }
// #endif // endif
// // BnS End
// //////////////////////////////////////////////////////////////////////////