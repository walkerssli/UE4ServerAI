// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/BehaviorTree.h"

UBehaviorTree::UBehaviorTree(const FString& name, int nodeMemory)
	: mName(name)
	, mNodeMemory(NULL)
	, mNodeMemorySize(nodeMemory)
	, mUsedMemorySize(0)
{
	if (nodeMemory > 0)
	{
		mNodeMemory = new char[nodeMemory];
		memset(mNodeMemory, 0, nodeMemory);
	}
}

UBehaviorTree::~UBehaviorTree()
{
	if (mNodeMemory)
	{
		delete(mNodeMemory);
		mNodeMemory = NULL;
	}
	mNodeMemorySize = 0;
	mUsedMemorySize = 0;
}

char* UBehaviorTree::GetNodeMemoryPtr(int sz)
{
	if (mNodeMemory)
	{
		if (mUsedMemorySize + sz > mNodeMemorySize || sz <= 0)
		{
			UE_VLOG("GetNodeMemoryPtr Error : TreeName:%s mNodeMemorySize:%d UsedSize:%d RequiredSize:%d", mName.c_str(), mNodeMemorySize, mUsedMemorySize, sz);
			return NULL;
		}
		char* ptr = mNodeMemory + mUsedMemorySize;
		mUsedMemorySize += sz;
		return ptr;
	}
	return NULL;
}

//
// //////////////////////////////////////////////////////////////////////////
// // BnS Begin: ID(ServerBehavior) modifier:(jarlmeng)
// #if WITH_EDITOR
// #include "XmlNode.h"
// #include "BehaviorTreeXmlHelper.h"
// void UBehaviorTree::ExportXml(class FXmlNode* Node, TSet<UBehaviorTree*>& SubTrees) const
// {
// 	Node->AppendAttribute(TEXT("Name"), GetName());
//
// 	auto TypesNode = BTXmlUtils::NewXmlNode(Node, TEXT("Types"));
// 	auto BlackboardNode = BTXmlUtils::NewXmlNode(Node, TEXT("Blackboard"));
// 	auto TreeNode = BTXmlUtils::NewXmlNode(Node, TEXT("Tree"));
//
// 	if (BlackboardAsset)
// 	{
// 		BlackboardAsset->ExportXml(BlackboardNode, TypesNode);
// 	}
//
// 	if (RootNode)
// 	{
// 		RootNode->ExportXml_Decorators(TreeNode, TypesNode, RootDecorators, RootDecoratorOps, SubTrees);
// 		RootNode->ExportXml(TreeNode, TypesNode, SubTrees);
// 	}
// }
// #endif // endif
// // BnS End
// //////////////////////////////////////////////////////////////////////////