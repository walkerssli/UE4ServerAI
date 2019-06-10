// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BTTaskNode.h"
//#include "AIController.h"
//#include "VisualLogger/VisualLogger.h"
//#include "GameplayTasksComponent.h"

UBTTaskNode::UBTTaskNode() : UBTNode()
{
	NodeType |= (uint32)BTNodeTypes_TaskNode;
	bNotifyTick = false;
	bNotifyTaskFinished = false;
	bIgnoreRestartSelf = false;
}


uint32 UBTTaskNode::sNodeTypeFlag = (uint32)BTNodeTypes_TaskNode;

EBTNodeResult::Type UBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTaskNode::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UBTTaskNode::WrappedExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBTNode* NodeOb = bCreateNodeInstance ? GetNodeInstance(OwnerComp, NodeMemory) : this;
	return NodeOb ? ((UBTTaskNode*)NodeOb)->ExecuteTask(OwnerComp, NodeMemory) : EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTaskNode::WrappedAbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBTNode* NodeOb = const_cast<UBTNode*>(bCreateNodeInstance ? GetNodeInstance(OwnerComp, NodeMemory) : this);
	UBTTaskNode* TaskNodeOb = static_cast<UBTTaskNode*>(NodeOb);
	EBTNodeResult::Type Result = TaskNodeOb ? TaskNodeOb->AbortTask(OwnerComp, NodeMemory) : EBTNodeResult::Aborted;

	return Result;
}

void UBTTaskNode::WrappedTickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) const
{
	if (bNotifyTick)
	{
		const UBTNode* NodeOb = bCreateNodeInstance ? GetNodeInstance(OwnerComp, NodeMemory) : this;
		if (NodeOb)
		{
			((UBTTaskNode*)NodeOb)->TickTask(OwnerComp, NodeMemory, DeltaSeconds);
		}
	}
}

void UBTTaskNode::WrappedOnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) const
{
	UBTNode* NodeOb = const_cast<UBTNode*>(bCreateNodeInstance ? GetNodeInstance(OwnerComp, NodeMemory) : this);

	if (NodeOb)
	{
		UBTTaskNode* TaskNodeOb = static_cast<UBTTaskNode*>(NodeOb);
		if (TaskNodeOb->bNotifyTaskFinished)
		{
			TaskNodeOb->OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
		}

// 		if (TaskNodeOb->bOwnsGameplayTasks && OwnerComp.GetAIOwner())
// 		{
// 			UGameplayTasksComponent* GTComp = OwnerComp.GetAIOwner()->GetGameplayTasksComponent();
// 			if (GTComp)
// 			{
// 				GTComp->EndAllResourceConsumingTasksOwnedBy(*TaskNodeOb);
// 			}
// 		}
	}
}

void UBTTaskNode::ReceivedMessage(UBehaviorTreeComponent* BrainComp, const FAIMessage& Message)
{
	UBehaviorTreeComponent* OwnerComp = static_cast<UBehaviorTreeComponent*>(BrainComp);
	//assert(OwnerComp);
	
	const uint16 InstanceIdx = OwnerComp->FindInstanceContainingNode(this);
	if (IsValidIndex(OwnerComp->InstanceStack,InstanceIdx))
	{
		uint8* NodeMemory = GetNodeMemory<uint8>(OwnerComp->InstanceStack[InstanceIdx]);
		OnMessage(*OwnerComp, NodeMemory, Message.MessageName, Message.RequestID, Message.Status == FAIMessage::Success);
	}
	else
	{
// 		UE_VLOG(OwnerComp->GetOwner(), LogBehaviorTree, Warning, TEXT("UBTTaskNode::ReceivedMessage called while %s node no longer in active BT")
// 			, *GetNodeName());
	}
}

void UBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// empty in base class
}

void UBTTaskNode::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// empty in base class
}

void UBTTaskNode::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
	const EBTTaskStatus::Type Status = OwnerComp.GetTaskStatus(this);
	if (Status == EBTTaskStatus::Active)
	{
		FinishLatentTask(OwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
	}
	else if (Status == EBTTaskStatus::Aborting)
	{
		FinishLatentAbort(OwnerComp);
	}
}

void UBTTaskNode::FinishLatentTask(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult) const
{
	// OnTaskFinished must receive valid template node

	UE_VLOG("UBTTaskNode::FinishLatentTask TaskName:%s\n", NodeName.c_str());
	UBTTaskNode* TemplateNode = (UBTTaskNode*)OwnerComp.FindTemplateNode(this);
	OwnerComp.OnTaskFinished(TemplateNode, TaskResult);
}

void UBTTaskNode::FinishLatentAbort(UBehaviorTreeComponent& OwnerComp) const
{
	// OnTaskFinished must receive valid template node
	UBTTaskNode* TemplateNode = (UBTTaskNode*)OwnerComp.FindTemplateNode(this);
	OwnerComp.OnTaskFinished(TemplateNode, EBTNodeResult::Aborted);
}

void UBTTaskNode::WaitForMessage(UBehaviorTreeComponent& OwnerComp, FName MessageType) const
{
	// messages delegates should be called on node instances (if they exists)
	OwnerComp.RegisterMessageObserver(this, MessageType);
}

void UBTTaskNode::WaitForMessage(UBehaviorTreeComponent& OwnerComp, FName MessageType, int32 RequestID) const
{
	// messages delegates should be called on node instances (if they exists)
	OwnerComp.RegisterMessageObserver(this, MessageType, RequestID);
}
	
void UBTTaskNode::StopWaitingForMessages(UBehaviorTreeComponent& OwnerComp) const
{
	// messages delegates should be called on node instances (if they exists)
	OwnerComp.UnregisterMessageObserversFrom(this);
}
// 
// #if WITH_EDITOR
// 
// FName UBTTaskNode::GetNodeIconName() const
// {
// 	return FName("BTEditor.Graph.BTNode.Task.Icon");
// }
// 
// #endif	// WITH_EDITOR

// void UBTTaskNode::OnGameplayTaskDeactivated(UGameplayTask& Task)
// {
// 	//assert(Task.GetTaskOwner() == this);
// 
// 	UBehaviorTreeComponent* BTComp = GetBTComponentForTask(Task);
// 	if (BTComp)
// 	{
// 		// this is a super-default behavior. Specific task will surely like to 
// 		// handle this themselves, finishing with specific result
// 		const EBTTaskStatus::Type Status = BTComp->GetTaskStatus(this);
// 		FinishLatentTask(*BTComp, Status == EBTTaskStatus::Aborting ? EBTNodeResult::Aborted : EBTNodeResult::Succeeded);
// 	}
// }

//----------------------------------------------------------------------//
// DEPRECATED
//----------------------------------------------------------------------//
// 
// //////////////////////////////////////////////////////////////////////////
// // BnS Begin: ID(ServerBehavior) modifier:(jarlmeng)
// #if WITH_EDITOR
// #include "XmlNode.h"
// #include "BehaviorTreeXmlHelper.h"
// FXmlNode* UBTTaskNode::ExportXml(class FXmlNode* ParentNode, class FXmlNode* TypesNode, TSet<UBehaviorTree*>& SubTrees) const
// {
// 	FXmlNode* Node = Super::ExportXml(ParentNode, TypesNode, SubTrees);
// 
// 	Node->AppendAttribute(TEXT("bIgnoreRestartSelf"), BTXmlUtils::GetBoolString(bIgnoreRestartSelf));
// 
// 	auto ServicesNode = BTXmlUtils::NewXmlNode(Node, TEXT("TaskServices"));
// 	for (const UBTService* Service : Services)
// 	{
// 		Service->ExportXml(ServicesNode, TypesNode, SubTrees);
// 	}
// 
// 	return Node;
// }
// #endif // endif
// // BnS End
// //////////////////////////////////////////////////////////////////////////