// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
//#include "UObject/ObjectMacros.h"
//#include "EngineDefines.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include <map>
#include <CoreTypes.h>
//#include "GameplayTagContainer.h"
//#include "AITypes.h"
//#include "BrainComponent.h"
//#include "BehaviorTreeComponent.generated.h"

class FBehaviorTreeDebugger;
class UBehaviorTree;
class UBTAuxiliaryNode;
class UBTCompositeNode;
class UBTDecorator;
class UBTNode;
class UBTTask_RunBehavior;
class UBTTask_RunBehaviorDynamic;
class UBTTaskNode;
struct FScopedBehaviorTreeLock;
class UBlackboardComponent;
class UBehaviorTreeComponent;
struct FAIMessage;
struct FAIRequestID;
class UE4ActorAgent;


DECLARE_DELEGATE_TwoParams(FOnAIMessage, UBehaviorTreeComponent*, const FAIMessage&);

struct FAIMessageInfo
{
	FOnAIMessage	mDelegate;
	FName			mMsgName;
	FAIRequestID	mRequestID;
	//FBTNodeIndex	mNodeIndex;
};

struct FBTNodeExecutionInfo
{
	/** index of first task allowed to be executed */
	FBTNodeIndex SearchStart;

	/** index of last task allowed to be executed */
	FBTNodeIndex SearchEnd;

	/** node to be executed */
	UBTCompositeNode* ExecuteNode;

	/** subtree index */
	uint16 ExecuteInstanceIdx;

	/** result used for resuming execution */
	EBTNodeResult::Type ContinueWithResult;

	/** if set, tree will try to execute next child of composite instead of forcing branch containing SearchStart */
	uint8 bTryNextChild : 1;

	/** if set, request was not instigated by finishing task/initialization but is a restart (e.g. decorator) */
	uint8 bIsRestart : 1;

	FBTNodeExecutionInfo() : ExecuteNode(NULL), bTryNextChild(false), bIsRestart(false) { }
};

struct FBTPendingExecutionInfo
{
	/** next task to execute */
	UBTTaskNode* NextTask;

	/** if set, tree ran out of nodes */
	uint32 bOutOfNodes : 1;

	/** if set, request can't be executed */
	uint32 bLocked : 1;

	FBTPendingExecutionInfo() : NextTask(NULL), bOutOfNodes(false), bLocked(false) {}
	bool IsSet() const { return (NextTask || bOutOfNodes) && !bLocked; }

	void Lock() { bLocked = true; }
	void Unlock() { bLocked = false; }
};

struct FBTTreeStartInfo
{
	FString TreePath;
	EBTExecutionMode::Type ExecuteMode;
	uint8 bPendingInitialize : 1;

	FBTTreeStartInfo() : ExecuteMode(EBTExecutionMode::Looped), bPendingInitialize(0) {}
	bool IsSet() const { return TreePath.length() > 0; }
	bool HasPendingInitialize() const { return bPendingInitialize && IsSet(); }
};

//UCLASS()
class UBehaviorTreeComponent //: public UBrainComponent
{
public:
	UBehaviorTreeComponent();
	virtual ~UBehaviorTreeComponent();
	//GENERATED_UCLASS_BODY()

	UE4ActorAgent* GetAgent();

	// Begin UBrainComponents
	virtual void RestartLogic() ;
	virtual void StopLogic(const FString& Reason) ;
	virtual void PauseLogic(const FString& Reason) ;
	//virtual EAILogicResuming::Type ResumeLogic(const FString& Reason);

	/** indicates instance has been initialized to work with specific BT asset */
	bool TreeHasBeenStarted() const;

public:
	/** DO NOT USE. This constructor is for internal usage only for hot-reload purposes. */
	//UBehaviorTreeComponent(FVTableHelper& Helper);

	virtual bool IsRunning() const ;
	virtual bool IsPaused() const ;
	virtual void Cleanup() ;
	// End UBrainComponents

	// Begin UActorComponents
	virtual void UninitializeComponent() ;
	// End UActorComponents

	/**
	 * [InitTreeBeforeStart 初始化行为树]
	 * @param  treePath [行为树路径]
	 * @param  iActorID [Actor唯一标识]
	 * @param  bb       [黑板数据地址]
	 * @param  iType    [黑板类型]
	 * @return          [0成功，非0失败]
	 */
	int InitTreeBeforeStart(const FString& treePath, int iActorID, void* bb, int &iType);

	/** starts execution from root */
	void StartTree(const FString& assetPath, EBTExecutionMode::Type ExecuteMode = EBTExecutionMode::Looped);

	/** stops execution */
	void StopTree(EBTStopMode::Type StopMode = EBTStopMode::Safe);

	/** restarts execution from root */
	void RestartTree();

	/** request execution change */
	void RequestExecution(UBTCompositeNode* RequestedOn, int32 InstanceIdx,
	                      const UBTNode* RequestedBy, int32 RequestedByChildIndex,
	                      EBTNodeResult::Type ContinueWithResult, bool bStoreForDebugger = true);

	/** request execution change: helpers for decorator nodes */
	void RequestExecution(const UBTDecorator* RequestedBy);

	/** request execution change: helpers for task nodes */
	void RequestExecution(EBTNodeResult::Type ContinueWithResult);

	/** finish latent execution or abort */
	void OnTaskFinished(const UBTTaskNode* TaskNode, EBTNodeResult::Type TaskResult);

	/** setup message observer for given task */
	void RegisterMessageObserver(const UBTTaskNode* TaskNode, FName MessageType);
	void RegisterMessageObserver(const UBTTaskNode* TaskNode, FName MessageType, FAIRequestID MessageID);

	void ReceiveAIMessage(FAIMessage& msg);

	/** remove message observers registered with task */
	void UnregisterMessageObserversFrom(const UBTTaskNode* TaskNode);
	//void UnregisterMessageObserversFrom(const FBTNodeIndex& TaskIdx);

	/** add active parallel task */
	void RegisterParallelTask(const UBTTaskNode* TaskNode);

	/** remove parallel task */
	void UnregisterParallelTask(const UBTTaskNode* TaskNode, uint16 InstanceIdx);

	/** unregister all aux nodes less important than given index */
	void UnregisterAuxNodesUpTo(const FBTNodeIndex& Index);

	/** unregister all aux nodes between given execution index range: FromIndex < AuxIndex < ToIndex */
	void UnregisterAuxNodesInRange(const FBTNodeIndex& FromIndex, const FBTNodeIndex& ToIndex);

	/** unregister all aux nodes in branch of tree */
	void UnregisterAuxNodesInBranch(const UBTCompositeNode* Node, bool bApplyImmediately = true);

	/** BEGIN UActorComponents */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType);
	/** END UActorComponents */

	/** process execution flow */
	void ProcessExecutionRequest();

	/** schedule execution flow update in next tick */
	void ScheduleExecutionUpdate();

	/** tries to find behavior tree instance in context */
	int32 FindInstanceContainingNode(const UBTNode* Node) const;

	/** tries to find template node for given instanced node */
	UBTNode* FindTemplateNode(const UBTNode* Node) const;

	/** @return current tree */
	inline UBehaviorTree* GetCurrentTree() const
	{
		return InstanceStack.size() ? KnownInstances[InstanceStack[ActiveInstanceIdx].InstanceIdIndex].TreeAsset : NULL;
	}

	/** @return tree from top of instance stack */
	inline UBehaviorTree* GetRootTree() const
	{
		return InstanceStack.size() ? KnownInstances[InstanceStack[0].InstanceIdIndex].TreeAsset : NULL;
	}

	/** @return active node */
	inline const UBTNode* GetActiveNode() const
	{
		return InstanceStack.size() ? InstanceStack[ActiveInstanceIdx].ActiveNode : NULL;
	}

	/** get index of active instance on stack */
	inline uint16 GetActiveInstanceIdx() const
	{
		return ActiveInstanceIdx;
	}

	/** @return node memory */
	uint8* GetNodeMemory(UBTNode* Node, int32 InstanceIdx) const;

	/** @return true if ExecutionRequest is switching to higher priority node */
	inline bool IsRestartPending() const
	{
		return ExecutionRequest.ExecuteNode && !ExecutionRequest.bTryNextChild;
	}

	/** @return true if waiting for abort to finish */
	inline bool IsAbortPending() const
	{
		return bWaitingForAbortingTasks || PendingExecution.IsSet();
	}

	/** @return true if active node is one of child nodes of given one */
	bool IsExecutingBranch(const UBTNode* Node, int32 ChildIndex = -1) const;

	/** @return true if aux node is currently active */
	bool IsAuxNodeActive(const UBTAuxiliaryNode* AuxNode) const;
	bool IsAuxNodeActive(const UBTAuxiliaryNode* AuxNodeTemplate, int32 InstanceIdx) const;

	/** Returns true if InstanceStack contains any BT runtime instances */
	bool IsInstanceStackEmpty() const { return (InstanceStack.size() == 0); }

	/** @return status of speficied task */
	EBTTaskStatus::Type GetTaskStatus(const UBTTaskNode* TaskNode) const;

	inline UBlackboardComponent* GetBlackboardComponent()
	{
		return BlackboardComp;
	}

// 	virtual FString GetDebugInfoString() const;
// 	virtual FString DescribeActiveTasks() const;
// 	virtual FString DescribeActiveTrees() const;

	/** @return the cooldown tag end time, 0.0f if CooldownTag is not found */
	//UFUNCTION(BlueprintCallable, Category = "AI|Logic")
	float GetTagCooldownEndTime(FGameplayTag CooldownTag) const;

	/** add to the cooldown tag's duration */
	//UFUNCTION(BlueprintCallable, Category = "AI|Logic")
	void AddCooldownTagDuration(FGameplayTag CooldownTag, float CooldownDuration, bool bAddToExistingDuration);

// 	/** assign subtree to RunBehaviorDynamic task specified by tag */
// 	//UFUNCTION(BlueprintCallable, Category="AI|Logic")
// 	virtual void SetDynamicSubtree(FGameplayTag InjectTag, UBehaviorTree* BehaviorAsset);
//
// #if ENABLE_VISUAL_LOG
// 	virtual void DescribeSelfToVisLog(struct FVisualLogEntry* Snapshot) const;
// #endif

protected:
	FString m_bbName;

	UBlackboardComponent* BlackboardComp;

	int mActorID;

	float mDeltaSeconds;
	inline float GetDeltaSeconds() const { return mDeltaSeconds; }

	UE4ActorAgent* m_pUE4ActorAgent;

	UBehaviorTree* m_pBehaviorTree;

	/** stack of behavior tree instances */
	std::vector<FBehaviorTreeInstance> InstanceStack;

	/** list of known subtree instances */
	std::vector<FBehaviorTreeInstanceId> KnownInstances;

	/** instanced nodes */
	std::vector<UBTNode*> NodeInstances;

	/** search data being currently used */
	FBehaviorTreeSearchData SearchData;

	/** execution request, search will be performed when current task finish execution/aborting */
	FBTNodeExecutionInfo ExecutionRequest;

	/** result of ExecutionRequest, will be applied when current task finish aborting */
	FBTPendingExecutionInfo PendingExecution;

	/** stored data for starting new tree, waits until previously running finishes aborting */
	FBTTreeStartInfo TreeStartInfo;

	/** message observers mapped by instance & execution index */
	typedef std::vector<FAIMessageInfo> AIMsgObserverArray;
	AIMsgObserverArray TaskMessageObservers;

	/** behavior cooldowns mapped by tag to last time it was set */
	typedef std::map<FGameplayTag, float> TypeCooldownTagMap;
	TypeCooldownTagMap CooldownTagsMap;
//
// #if USE_BEHAVIORTREE_DEBUGGER
// 	/** search flow for debugger */
// 	mutable std::vector<std::vector<FBehaviorTreeDebuggerInstance::FNodeFlowData> > CurrentSearchFlow;
// 	mutable std::vector<std::vector<FBehaviorTreeDebuggerInstance::FNodeFlowData> > CurrentRestarts;
// 	mutable TMap<FName, FString> SearchStartBlackboard;
// 	mutable std::vector<FBehaviorTreeDebuggerInstance> RemovedInstances;
//
// 	/** debugger's recorded data */
// 	mutable std::vector<FBehaviorTreeExecutionStep> DebuggerSteps;
//
// 	/** set when at least one debugger window is opened */
// 	static int32 ActiveDebuggerCounter;
// #endif

	/** index of last active instance on stack */
	uint16 ActiveInstanceIdx;

	/** if set, StopTree calls will be deferred */
	uint8 StopTreeLock;

	/** if set, StopTree will be called at the end of tick */
	uint8 bDeferredStopTree : 1;

	/** loops tree execution */
	uint8 bLoopExecution : 1;

	/** set when execution is waiting for tasks to abort (current or parallel's main) */
	uint8 bWaitingForAbortingTasks : 1;

	/** set when execution update is scheduled for next tick */
	uint8 bRequestedFlowUpdate : 1;

	/** set when tree stop was called */
	uint8 bRequestedStop : 1;

	/** if set, tree execution is allowed */
	uint8 bIsRunning : 1;

	/** if set, execution requests will be postponed */
	uint8 bIsPaused : 1;

	/** push behavior tree instance on execution stack
	 *	@NOTE: should never be called out-side of BT execution, meaning only BT tasks can push another BT instance! */
	bool PushInstance(const FString& treePath);

	/** add unique Id of newly created subtree to KnownInstances list and return its index */
	uint8 UpdateInstanceId(UBehaviorTree* TreeAsset, const UBTNode* OriginNode, int32 OriginInstanceIdx);

	/** remove instanced nodes, known subtree instances and safely clears their persistent memory */
	void RemoveAllInstances();

	/** copy memory block from running instances to persistent memory */
	void CopyInstanceMemoryToPersistent();

	/** copy memory block from persistent memory to running instances (rollback) */
	void CopyInstanceMemoryFromPersistent();

	/** find next task to execute */
	UBTTaskNode* FindNextTask(UBTCompositeNode* ParentNode, uint16 ParentInstanceIdx, EBTNodeResult::Type LastResult);

	/** called when tree runs out of nodes to execute */
	void OnTreeFinished();

	/** apply pending node updates from SearchData */
	void ApplySearchData(UBTNode* NewActiveNode);

	/** apply pending node updates required for discarded search */
	void ApplyDiscardedSearch();

	/** apply updates from specific list */
	void ApplySearchUpdates(const std::vector<FBehaviorTreeSearchUpdate>& UpdateList, int32 NewNodeExecutionIndex, bool bPostUpdate = false);

	/** abort currently executed task */
	void AbortCurrentTask();

	/** execute new task */
	void ExecuteTask(UBTTaskNode* TaskNode);

	/** deactivate all nodes up to requested one */
	bool DeactivateUpTo(UBTCompositeNode* Node, uint16 NodeInstanceIdx, EBTNodeResult::Type& NodeResult);

	/** update state of aborting tasks */
	void UpdateAbortingTasks();

	/** apply pending execution from last task search */
	void ProcessPendingExecution();

	/** apply pending tree initialization */
	void ProcessPendingInitialize();

	/** restore state of tree to state before search */
	void RollbackSearchChanges();
//
// 	/** make a snapshot for debugger */
// 	void StoreDebuggerExecutionStep(EBTExecutionSnap::Type SnapType);
//
// 	/** make a snapshot for debugger from given subtree instance */
// 	void StoreDebuggerInstance(FBehaviorTreeDebuggerInstance& InstanceInfo, uint16 InstanceIdx, EBTExecutionSnap::Type SnapType) const;
// 	void StoreDebuggerRemovedInstance(uint16 InstanceIdx) const;
//
// 	/** store search step for debugger */
// 	void StoreDebuggerSearchStep(const UBTNode* Node, uint16 InstanceIdx, EBTNodeResult::Type NodeResult) const;
// 	void StoreDebuggerSearchStep(const UBTNode* Node, uint16 InstanceIdx, bool bPassed) const;
//
// 	/** store restarting node for debugger */
// 	void StoreDebuggerRestart(const UBTNode* Node, uint16 InstanceIdx, bool bAllowed);
//
// 	/** describe blackboard's key values */
// 	void StoreDebuggerBlackboard(TMap<FName, FString>& BlackboardValueDesc) const;
//
// 	/** gather nodes runtime descriptions */
// 	void StoreDebuggerRuntimeValues(std::vector<FString>& RuntimeDescriptions, UBTNode* RootNode, uint16 InstanceIdx) const;
//
// 	/** update runtime description of given task node in latest debugger's snapshot */
// 	void UpdateDebuggerAfterExecution(const UBTTaskNode* TaskNode, uint16 InstanceIdx) const;
//
// 	/** check if debugger is currently running and can gather data */
// 	static bool IsDebuggerActive();

	EBTNodeRelativePriority CalculateRelativePriority(const UBTNode* NodeA, const UBTNode* NodeB) const;


	friend class UBTNode;
	friend class UBTCompositeNode;
	friend class UBTTaskNode;
	friend class UBTTask_RunBehavior;
	friend class UBTTask_RunBehaviorDynamic;
	friend class FBehaviorTreeDebugger;
	friend class FBehaviorTreeInstance;
	friend class FScopedBehaviorTreeLock;
};
