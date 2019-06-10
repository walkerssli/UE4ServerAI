// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
//#include "UObject/ObjectMacros.h"
//#include "UObject/Object.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include <assert.h>
//#include "GameplayTaskOwnerInterface.h"
//#include "Tasks/AITask.h"
//#include "BTNode.generated.h"

//class AActor;
class UBehaviorTree;
class UBlackboardData;
class UBTCompositeNode;
//class UGameplayTasksComponent;

//TODO_BNS
//AIMODULE_API DECLARE_LOG_CATEGORY_EXTERN(LogBehaviorTree, Display, All);

//class AAIController;
//class UWorld;
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBTCompositeNode;
class UBlackboardData;
class UBTService;
struct FBehaviorTreeSearchData;

struct FBTInstancedNodeMemory
{
	int32 NodeIdx;
};

class UBTNode
{
public:
	UBTNode();

	static uint32 sNodeTypeFlag;
	
	std::vector<UBTService*> Services;

	/**
	* Do any object-specific cleanup required immediately after loading an object,
	* and immediately after any undo/redo.
	*/
	virtual void PostLoad() {};

	/** fill in data about tree structure */
	void InitializeNode(UBTCompositeNode* InParentNode, uint16 InExecutionIndex, uint16 InMemoryOffset, uint8 InTreeDepth);

	virtual FString GetDescription() const;
	/** size of instance memory */
	virtual uint16 GetInstanceMemorySize() const;

	/** called when node instance is added to tree */
	virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp);

	/** called when node instance is removed from tree */
	virtual void OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp);

	/** called on creating subtree to set up memory and instancing */
	void InitializeInSubtree(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, int32& NextInstancedIndex, EBTMemoryInit::Type InitType) const;

	/** called on removing subtree to cleanup memory */
	void CleanupInSubtree(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const;

	/** size of special, hidden memory block for internal mechanics */
	virtual uint16 GetSpecialMemorySize() const;

	template<typename T>
	T* GetNodeMemory(FBehaviorTreeSearchData& SearchData) const
	{
		return NULL;
	}

	template<typename T>
	const T* GetNodeMemory(const FBehaviorTreeSearchData& SearchData) const
	{
		return NULL;
	}

	template<typename T>
	T* GetNodeMemory(FBehaviorTreeInstance& BTInstance) const
	{
		return NULL;
	}

	template<typename T>
	const T* GetNodeMemory(const FBehaviorTreeInstance& BTInstance) const
	{
		return NULL;
	}

	/** get special memory block used for hidden shared data (e.g. node instancing) */
	template<typename T>
	T* GetSpecialNodeMemory(uint8* NodeMemory) const
	{
		return NULL;
	}

	/** @return parent node */
	inline UBTCompositeNode* GetParentNode() const
	{
		return ParentNode;
	}

	/** @return name of node */
	FString GetNodeName() const;

	/** @return execution index */
	inline uint16 GetExecutionIndex() const
	{
		return ExecutionIndex;
	}

	/** @return memory offset */
	inline uint16 GetMemoryOffset() const
	{
		return MemoryOffset;
	}

	/** @return depth in tree */
	inline uint8 GetTreeDepth() const
	{
		return TreeDepth;
	}

	/** sets bIsInjected flag, do NOT call this function unless you really know what you are doing! */
	inline void MarkInjectedNode()
	{
		bIsInjected = true;
	}

	/** @return true if node was injected by subtree */
	inline bool IsInjected() const
	{
		return bIsInjected;
	}


	/** sets bCreateNodeInstance flag, do NOT call this function on already pushed tree instance! */
	inline void ForceInstancing(bool bEnable)
	{
		// allow only in not initialized trees, side effect: root node always blocked
		assert(ParentNode == NULL);

		bCreateNodeInstance = bEnable;
	}

	/** @return true if node wants to be instanced */
	inline bool HasInstance() const
	{
		return bCreateNodeInstance;
	}

	/** @return true if this object is instanced node */
	inline bool IsInstanced() const
	{
		return bIsInstanced;
	}
	
	/** @return node instance if bCreateNodeInstance was set */
	const UBTNode* GetNodeInstance(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	const UBTNode* GetNodeInstance(FBehaviorTreeSearchData& SearchData) const;
	UBTNode* GetNodeInstance(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	UBTNode* GetNodeInstance(FBehaviorTreeSearchData& SearchData);
	
	FString NodeName;
	uint32 NodeType;
	FString NodeTypeName;
	inline uint32 GetTypeFlag() const { return NodeType;}
	inline const FString& GetNodeTypeName() const { return NodeTypeName; }
public:
//
// 	/** source asset */
// 	UPROPERTY()
// 		UBehaviorTree* TreeAsset;

	/** parent node */
	UBTCompositeNode* ParentNode;
	
	/** depth first index (execution order) */
	uint16 ExecutionIndex;

	/** instance memory offset */
	uint16 MemoryOffset;

	/** depth in tree */
	uint8 TreeDepth;

	/** set automatically for node instances. Should never be set manually */
	uint8 bIsInstanced : 1;

	/** if set, node is injected by subtree. Should never be set manually */
	uint8 bIsInjected : 1;

protected:

	/** if set, node will be instanced instead of using memory block and template shared with all other BT components */
	uint8 bCreateNodeInstance : 1;

	/** set to true if task owns any GameplayTasks. Note this requires tasks to be created via NewBTAITask
	*	Otherwise specific BT task node class is responsible for ending the gameplay tasks on node finish */
	uint8 bOwnsGameplayTasks : 1;

};
