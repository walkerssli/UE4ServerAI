#pragma once

#include "Core/NumericLimits.h"
#include "Core/MyDelegate.h"
#include "Core/CoreTypes.h"

#include <string>
#include <vector>
#include <stdarg.h>
#include <time.h>
using namespace std;

enum { INDEX_NONE = -1 };

//class FBlackboardDecoratorDetails;
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBlackboardData;
class UBlackboardKeyType;
class UBTAuxiliaryNode;
class UBTCompositeNode;
class UBTNode;
class UBTTaskNode;
struct FBehaviorTreeSearchData;

typedef std::string FName;
typedef std::string FString;
typedef std::string FBlackboardKeySelector;

#define IsValidIndex(vec, idx)	(idx >=0 && idx < (int)vec.size())
#define LastOfVector(vec) (vec[vec.size()-1])
//#define TEXT(s) (s)
#ifdef ENABLE_UE_LOG
#define UE_VLOG(FormatStr, ...) printf(FormatStr, ##__VA_ARGS__)
#else
#define UE_VLOG(FormatStr, ...)
#endif
// #define UE_CVLOG(Condition, LogOwner, CategoryName, Verbosity, Format, ...) ()
// #define BT_VLOG(Context, Verbosity, Format, ...) UE_VLOG(Context->OwnerComp.IsValid() ? Context->OwnerComp->GetOwner() : NULL, LogBehaviorTree, Verbosity, Format, ##__VA_ARGS__)
// #define BT_SEARCHLOG(SearchData, Verbosity, Format, ...) UE_VLOG(SearchData.OwnerComp.GetOwner(), LogBehaviorTree, Verbosity, Format, ##__VA_ARGS__)
//
typedef std::string FGameplayTag;

static FString Printf(const char* _Format, ...)
{
	char tmp[256];
	va_list _ArgList;
	va_start(_ArgList, _Format);
	sprintf_s(tmp, 256, _Format, _ArgList);
	va_end(_ArgList);
	return FString(tmp);
}

namespace FBlackboard
{
const FName KeySelf = "SelfActor";

typedef std::string FKey;

const FKey InvalidKey = "";

}

enum EBlackboardNotificationResult
{
	EBlackboardNotificationResult_RemoveObserver,
	EBlackboardNotificationResult_ContinueObserving
};

enum BTNodeTypes
{
	BTNodeTypes_Node = 0x00000001,
	BTNodeTypes_CompositeNode = 0x00000002,
	BTNodeTypes_AuxiliaryNode = 0x00000004,
	BTNodeTypes_Decorator = 0x00000008,
	BTNodeTypes_Service = 0x00000010,
	BTNodeTypes_TaskNode = 0x00000020,
};

uint32 GetNodeTypeFlag(const UBTNode* node);

template<typename TargetType>
static TargetType* CastNode(UBTNode* node)
{
	uint32 typeFlag = GetNodeTypeFlag(node);
	if ((typeFlag & TargetType::sNodeTypeFlag) != 0)
	{
		return static_cast<TargetType*>(node);
	}
	return NULL;
}

template<typename TargetType>
static bool IsANodeType(const UBTNode* node)
{
	uint32 typeFlag = GetNodeTypeFlag(node);
	return ((typeFlag & TargetType::sNodeTypeFlag) != 0);
}

template<typename ElementType>
static bool VectorContains(const std::vector<ElementType>& vec, const ElementType& ele)
{
	for (unsigned i = 0; i < vec.size(); ++i)
	{
		if (vec[i] == ele)
		{
			return true;
		}
	}
	return false;
}

template<typename ElementType>
static unsigned IndexOfVector(const std::vector<ElementType>& vec, const ElementType& ele)
{
	for (unsigned i = 0; i < vec.size(); ++i)
	{
		if (vec[i] == ele)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

template<typename ElementType>
static bool EqualVector(const std::vector<ElementType>& vec1, const std::vector<ElementType>& vec2)
{
	if (vec1.size() == vec2.size())
	{
		for (unsigned i = 0; i < vec1.size(); ++i)
		{
			if (vec1[i] != vec2[i])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}





struct FVector
{
public:
	FVector(): X(0.0f), Y(0.0f), Z(0.0f)
	{}

	FVector(float x, float y, float z): X(x), Y(y), Z(z)
	{}

	FVector(float v) : X(v), Y(v), Z(v)
	{}

	inline FVector& operator=(const FVector& Other)
	{
		this->X = Other.X;
		this->Y = Other.Y;
		this->Z = Other.Z;
		return *this;
	}
	inline bool operator==(const FVector& V) const
	{
		return X == V.X && Y == V.Y && Z == V.Z;
	}
	inline bool operator!=(const FVector& V) const
	{
		return X != V.X || Y != V.Y || Z != V.Z;
	}


	static const FVector InvalidVector;

	float X;
	float Y;
	float Z;
};

struct FRotator
{
public:
	FRotator(float InPitch, float InYaw, float InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll)
	{
	}

	FRotator(float v) : Pitch(v), Yaw(v), Roll(v)
	{}

	FRotator(const FRotator& V) : Pitch(V.Pitch), Yaw(V.Yaw), Roll(V.Roll)
	{}

	inline bool operator==(const FRotator& V) const
	{
		return Pitch == V.Pitch && Yaw == V.Yaw && Roll == V.Roll;
	}
	inline bool operator!=(const FRotator& V) const
	{
		return Pitch != V.Pitch || Yaw != V.Yaw || Roll != V.Roll;
	}

	/** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
	float Pitch;

	/** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
	float Yaw;

	/** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
	float Roll;

	static const FRotator InvalidRotator;

};

struct FAIRequestID
{

private:
	static const uint32 AnyRequestID;
	static const uint32 InvalidRequestID;

	uint32 RequestID;

public:
	FAIRequestID(uint32 InRequestID = InvalidRequestID) : RequestID(InRequestID)
	{}

	/** returns true if given ID is identical to stored ID or any of considered
	*	IDs is FAIRequestID::AnyRequest*/
	inline bool IsEquivalent(uint32 OtherID) const
	{
		return OtherID != InvalidRequestID && this->IsValid() && (RequestID == OtherID || RequestID == AnyRequestID || OtherID == AnyRequestID);
	}

	inline bool IsEquivalent(FAIRequestID Other) const
	{
		return IsEquivalent(Other.RequestID);
	}

	inline bool IsValid() const
	{
		return RequestID != InvalidRequestID;
	}

	inline uint32 GetID() const { return RequestID; }

	void operator=(uint32 OtherID)
	{
		RequestID = OtherID;
	}

	operator uint32() const
	{
		return RequestID;
	}

	FString ToString() const
	{
		return Printf("%d", int32(RequestID));
	}

	static const FAIRequestID AnyRequest;
	static const FAIRequestID CurrentRequest;
	static const FAIRequestID InvalidRequest;
};

struct FAIMessage
{
	enum EStatus
	{
		Failure,
		Success,
	};

	/** type of message */
	FName MessageName;

	/** message source */
	//FWeakObjectPtr Sender;

	/** message param: ID */
	FAIRequestID RequestID;

	/** message param: status */
	EStatus Status;

	/** message param: custom flags */
	uint8 MessageFlags;

	FAIMessage() : RequestID(0), Status(FAIMessage::Success), MessageFlags(0) {}
	FAIMessage(FName InMessage, bool bSuccess) : MessageName(InMessage), RequestID(0), Status(bSuccess ? Success : Failure), MessageFlags(0) {}
	FAIMessage(FName InMessage, FAIRequestID InID, bool bSuccess) : MessageName(InMessage), RequestID(InID), Status(bSuccess ? Success : Failure), MessageFlags(0) {}
// 	FAIMessage(FName InMessage) : MessageName(InMessage), Sender(InSender), RequestID(0), Status(FAIMessage::Success), MessageFlags(0) {}
// 	FAIMessage(FName InMessage, FAIRequestID InID, EStatus InStatus) : MessageName(InMessage), Sender(InSender), RequestID(InID), Status(InStatus), MessageFlags(0) {}
// 	FAIMessage(FName InMessage, EStatus InStatus) : MessageName(InMessage), Sender(InSender), RequestID(0), Status(InStatus), MessageFlags(0) {}
// 	FAIMessage(FName InMessage, bool bSuccess) : MessageName(InMessage), Sender(InSender), RequestID(0), Status(bSuccess ? Success : Failure), MessageFlags(0) {}

	void SetFlags(uint8 Flags) { MessageFlags = Flags; }
	void SetFlag(uint8 Flag) { MessageFlags |= Flag; }
	void ClearFlag(uint8 Flag) { MessageFlags &= ~Flag; }
	bool HasFlag(uint8 Flag) const { return (MessageFlags & Flag) != 0; }

// 	static void Send(AController* Controller, const FAIMessage& Message);
// 	static void Send(APawn* Pawn, const FAIMessage& Message);
// 	static void Send(UBrainComponent* BrainComp, const FAIMessage& Message);
//	static void Broadcast(const FAIMessage& Message);
};

namespace FMath
{
template< class T >
static inline T Max(const T A, const T B)
{
	return (A >= B) ? A : B;
}
template< class T >
static inline T Min(const T A, const T B)
{
	return (A <= B) ? A : B;
}

static inline float FRandRange(float InMin, float InMax)
{
	return InMin + (InMax - InMin) * (rand() % 1000) / 1000.0f;
}
}
namespace FBTTime
{
static inline float GetTimeSeconds()
{
	clock_t t = clock();
	return (float)((double)t / (double)CLOCKS_PER_SEC);
}
}

namespace BTSpecialChild
{
const int32 NotInitialized = -1;	// special value for child indices: needs to be initialized
const int32 ReturnToParent = -2;	// special value for child indices: return to parent node

const uint8 OwnedByComposite = MAX_uint8;	// special value for aux node's child index: owned by composite node instead of a task
}

namespace EBTNodeResult
{
// keep in sync with DescribeNodeResult()
enum Type
{
	Succeeded,		// finished as success
	Failed,			// finished as failure
	Aborted,		// finished aborting = failure
	InProgress,		// not finished yet
};
}

namespace EBTExecutionMode
{
enum Type
{
	SingleRun,
	Looped,
};
}

namespace EBTStopMode
{
enum Type
{
	Safe,
	Forced,
};
}

namespace EBTMemoryInit
{
enum Type
{
	Initialize,		// first time initialization
	RestoreSubtree,	// loading saved data on reentering subtree
};
}

namespace EBTMemoryClear
{
enum Type
{
	Destroy,		// final clear
	StoreSubtree,	// saving data on leaving subtree
};
}

namespace EBTFlowAbortMode
{
// keep in sync with DescribeFlowAbortMode()

enum Type
{
	None,
	LowerPriority,
	Self,
	Both,
};
}

namespace EBTActiveNode
{
// keep in sync with DescribeActiveNode()
enum Type
{
	Composite,
	ActiveTask,
	AbortingTask,
	InactiveTask,
};
}

namespace EBTTaskStatus
{
// keep in sync with DescribeTaskStatus()
enum Type
{
	Active,
	Aborting,
	Inactive,
};
}

namespace EBTNodeUpdateMode
{
// keep in sync with DescribeNodeUpdateMode()
enum Type
{
	Unknown,
	Add,				// add node
	Remove,				// remove node
};
}

//TODO_BNS
DECLARE_DELEGATE_TwoParams(FOnBlackboardChange, const UBlackboardComponent&, FBlackboard::FKey /*key ID*/);
DECLARE_DELEGATE_RetVal_TwoParams(EBlackboardNotificationResult, FOnBlackboardChangeNotification, const UBlackboardComponent&, FBlackboard::FKey /*key ID*/);
DECLARE_DELEGATE_TwoParams(FBTInstanceDeactivation, UBehaviorTreeComponent&, EBTNodeResult::Type);

/** wrapper struct for holding a parallel task node and its status */
struct FBehaviorTreeParallelTask
{
	/** worker object */
	const UBTTaskNode* TaskNode;

	/** additional mode data used for context switching */
	EBTTaskStatus::Type Status;

	FBehaviorTreeParallelTask() : TaskNode(NULL) {}
	FBehaviorTreeParallelTask(const UBTTaskNode* InTaskNode, EBTTaskStatus::Type InStatus) : TaskNode(InTaskNode), Status(InStatus) {}

	bool operator==(const FBehaviorTreeParallelTask& Other) const { return TaskNode == Other.TaskNode; }
	bool operator==(const UBTTaskNode* OtherTask) const { return TaskNode == OtherTask; }
};

namespace EBTExecutionSnap
{
enum Type
{
	Regular,
	OutOfNodes,
};
}

namespace EBTDescriptionVerbosity
{
enum Type
{
	Basic,
	Detailed,
};
}

enum EBTNodeRelativePriority
{
	EBTNodeRelativePriority_Lower,
	EBTNodeRelativePriority_Same,
	EBTNodeRelativePriority_Higher
};

// /** debugger data about subtree instance */
// struct FBehaviorTreeDebuggerInstance
// {
// 	struct FNodeFlowData
// 	{
// 		uint16 ExecutionIndex;
// 		uint16 bPassed : 1;
// 		uint16 bTrigger : 1;
// 		uint16 bDiscardedTrigger : 1;
//
// 		FNodeFlowData() : ExecutionIndex(INDEX_NONE), bPassed(0), bTrigger(0), bDiscardedTrigger(0) {}
// 	};
//
// 	FBehaviorTreeDebuggerInstance() : TreeAsset(NULL), RootNode(NULL) {}
//
// 	/** behavior tree asset */
// 	UBehaviorTree* TreeAsset;
//
// 	/** root node in template */
// 	UBTCompositeNode* RootNode;
//
// 	/** execution indices of active nodes */
// 	std::vector<uint16> ActivePath;
//
// 	/** execution indices of active nodes */
// 	std::vector<uint16> AdditionalActiveNodes;
//
// 	/** search flow from previous state */
// 	std::vector<FNodeFlowData> PathFromPrevious;
//
// 	/** runtime descriptions for each execution index */
// 	std::vector<FString> RuntimeDesc;
//
// 	FORCEINLINE bool IsValid() const { return ActivePath.size() != 0; }
// };

// /** debugger data about current execution step */
// struct FBehaviorTreeExecutionStep
// {
// 	FBehaviorTreeExecutionStep() : TimeStamp(0.f), StepIndex(INDEX_NONE) {}
//
// 	/** subtree instance stack */
// 	std::vector<FBehaviorTreeDebuggerInstance> InstanceStack;
//
// 	/** blackboard snapshot: value descriptions */
// 	TMap<FName, FString> BlackboardValues;
//
// 	/** Game world's time stamp of this step */
// 	float TimeStamp;
//
// 	/** index of execution step */
// 	int32 StepIndex;
// };

/** identifier of subtree instance */
struct FBehaviorTreeInstanceId
{
	/** behavior tree asset */
	UBehaviorTree* TreeAsset;

	/** root node in template for cleanup purposes */
	UBTCompositeNode* RootNode;

	/** execution index path from root */
	vector<uint16> Path;

	/** persistent instance memory */
	std::vector<uint8> InstanceMemory;

	/** index of first node instance (BehaviorTreeComponent.NodeInstances) */
	int32 FirstNodeInstance;

	FBehaviorTreeInstanceId() : TreeAsset(0), RootNode(0), FirstNodeInstance(-1) {}

	bool operator==(const FBehaviorTreeInstanceId& Other) const;
};

/** data required for instance of single subtree */
struct FBehaviorTreeInstance
{
	/** root node in template */
	UBTCompositeNode* RootNode;

	/** active node in template */
	UBTNode* ActiveNode;

	std::vector<UBTNode*> AllNodes;

	/** active auxiliary nodes */
	std::vector<UBTAuxiliaryNode*> ActiveAuxNodes;

	/** active parallel tasks */
	std::vector<FBehaviorTreeParallelTask> ParallelTasks;

	/** memory: instance */
	std::vector<uint8> InstanceMemory;

	/** index of identifier (BehaviorTreeComponent.KnownInstances) */
	uint8 InstanceIdIndex;

	/** active node type */
	EBTActiveNode::Type ActiveNodeType;

	/** delegate sending a notify when tree instance is removed from active stack */
	FBTInstanceDeactivation DeactivationNotify;

	FBehaviorTreeInstance() { IncMemoryStats(); }
	FBehaviorTreeInstance(const FBehaviorTreeInstance& Other) { *this = Other; IncMemoryStats(); }
	FBehaviorTreeInstance(int32 MemorySize) { /*InstanceMemory.AddZeroed(MemorySize);*/ IncMemoryStats(); }
	~FBehaviorTreeInstance() { DecMemoryStats(); }

	inline uint32 GetAllocatedSize() const { return 0; }
	inline void IncMemoryStats() {}
	inline void DecMemoryStats() {}

	/** initialize memory and create node instances */
	void Initialize(UBehaviorTreeComponent& OwnerComp, UBTCompositeNode& Node, int32& InstancedIndex, EBTMemoryInit::Type InitType);

	/** cleanup node instances */
	void Cleanup(UBehaviorTreeComponent& OwnerComp, EBTMemoryClear::Type CleanupType);

	/** check if instance has active node with given execution index */
	bool HasActiveNode(uint16 TestExecutionIndex) const;

	/** deactivate all active aux nodes and remove their requests from SearchData */
	void DeactivateNodes(FBehaviorTreeSearchData& SearchData, uint16 InstanceIndex);

protected:

	/** worker for updating all nodes */
	void CleanupNodes(UBehaviorTreeComponent& OwnerComp, UBTCompositeNode& Node, EBTMemoryClear::Type CleanupType);
};

struct FBTNodeIndex
{
	/** index of instance of stack */
	uint16 InstanceIndex;

	/** execution index within instance */
	uint16 ExecutionIndex;

	FBTNodeIndex() : InstanceIndex(MAX_uint16), ExecutionIndex(MAX_uint16) {}
	FBTNodeIndex(uint16 InInstanceIndex, uint16 InExecutionIndex) : InstanceIndex(InInstanceIndex), ExecutionIndex(InExecutionIndex) {}

	bool TakesPriorityOver(const FBTNodeIndex& Other) const;
	bool IsSet() const { return InstanceIndex < MAX_uint16; }

	inline bool operator==(const FBTNodeIndex& Other) const { return Other.ExecutionIndex == ExecutionIndex && Other.InstanceIndex == InstanceIndex; }
	inline friend uint32 GetTypeHash(const FBTNodeIndex& Other) { return Other.ExecutionIndex ^ Other.InstanceIndex; }

	//inline FString Describe() const { return Printf(TEXT("[%d:%d]"), InstanceIndex, ExecutionIndex); }
};

/** node update data */
struct FBehaviorTreeSearchUpdate
{
	UBTAuxiliaryNode* AuxNode;
	UBTTaskNode* TaskNode;

	uint16 InstanceIndex;

	EBTNodeUpdateMode::Type Mode;

	/** if set, this entry will be applied AFTER other are processed */
	uint8 bPostUpdate : 1;

	FBehaviorTreeSearchUpdate() : AuxNode(0), TaskNode(0), InstanceIndex(0), Mode(EBTNodeUpdateMode::Unknown), bPostUpdate(false) {}
	FBehaviorTreeSearchUpdate(const UBTAuxiliaryNode* InAuxNode, uint16 InInstanceIndex, EBTNodeUpdateMode::Type InMode) :
		AuxNode((UBTAuxiliaryNode*)InAuxNode), TaskNode(0), InstanceIndex(InInstanceIndex), Mode(InMode), bPostUpdate(false)
	{}
	FBehaviorTreeSearchUpdate(const UBTTaskNode* InTaskNode, uint16 InInstanceIndex, EBTNodeUpdateMode::Type InMode) :
		AuxNode(0), TaskNode((UBTTaskNode*)InTaskNode), InstanceIndex(InInstanceIndex), Mode(InMode), bPostUpdate(false)
	{}
};

/** instance notify data */
struct FBehaviorTreeSearchUpdateNotify
{
	uint16 InstanceIndex;
	EBTNodeResult::Type NodeResult;

	FBehaviorTreeSearchUpdateNotify() : InstanceIndex(0), NodeResult(EBTNodeResult::Succeeded) {}
	FBehaviorTreeSearchUpdateNotify(uint16 InInstanceIndex, EBTNodeResult::Type InNodeResult) : InstanceIndex(InInstanceIndex), NodeResult(InNodeResult) {}
};

/** node search data */
struct FBehaviorTreeSearchData
{
	/** BT component */
	UBehaviorTreeComponent& OwnerComp;

	/** requested updates of additional nodes (preconditions, services, parallels)
	*  buffered during search to prevent instant add & remove pairs */
	std::vector<FBehaviorTreeSearchUpdate> PendingUpdates;

	/** notifies for tree instances */
	std::vector<FBehaviorTreeSearchUpdateNotify> PendingNotifies;

	/** first node allowed in search */
	FBTNodeIndex SearchStart;

	/** last node allowed in search */
	FBTNodeIndex SearchEnd;

	/** search unique number */
	int32 SearchId;

	/** active instance index to rollback to */
	int32 RollbackInstanceIdx;

	/** if set, current search will be restarted in next tick */
	uint32 bPostponeSearch : 1;

	/** set when task search is in progress */
	uint32 bSearchInProgress : 1;

	/** if set, active node state/memory won't be rolled back */
	uint32 bPreserveActiveNodeMemoryOnRollback : 1;

	/** adds update info to PendingUpdates array, removing all previous updates for this node */
	void AddUniqueUpdate(const FBehaviorTreeSearchUpdate& UpdateInfo);

	/** assign unique Id number */
	void AssignSearchId();

	/** clear state of search */
	void Reset();

	FBehaviorTreeSearchData(UBehaviorTreeComponent& InOwnerComp)
		: OwnerComp(InOwnerComp), RollbackInstanceIdx(INDEX_NONE), bPostponeSearch(false), bSearchInProgress(false), bPreserveActiveNodeMemoryOnRollback(false)
	{}

private:

	static int32 NextSearchId;
};
//
// /** property block in blueprint defined nodes */
// struct FBehaviorTreePropertyMemory
// {
// 	uint16 Offset;
// 	uint16 BlockSize;
//
// 	FBehaviorTreePropertyMemory() {}
// 	FBehaviorTreePropertyMemory(int32 Value) : Offset((uint32)Value >> 16), BlockSize((uint32)Value & 0xFFFF) {}
//
// 	int32 Pack() const { return (int32)(((uint32)Offset << 16) | BlockSize); }
// };
//
// /** helper struct for defining types of allowed blackboard entries
// *  (e.g. only entries holding points and objects derived form actor class) */
// USTRUCT(BlueprintType)
// struct AIMODULE_API FBlackboardKeySelector
// {
// 	GENERATED_USTRUCT_BODY()
//
// 		FBlackboardKeySelector() : SelectedKeyID(FBlackboard::InvalidKey), bNoneIsAllowedValue(false)
// 	{}
//
// 	/** array of allowed types with additional properties (e.g. uobject's base class)
// 	* EditAnywhere is required for FBlackboardSelectorDetails::CacheBlackboardData() */
// 	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Blackboard)
// 		std::vector<UBlackboardKeyType*> AllowedTypes;
//
// 	/** name of selected key */
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
// 		FName SelectedKeyName;
//
// 	/** class of selected key  */
// 	UPROPERTY(transient, EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
// 		TSubclassOf<UBlackboardKeyType> SelectedKeyType;
//
// protected:
// 	/** ID of selected key */
// 	UPROPERTY(transient, EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
// 		uint8 SelectedKeyID;
// 	// SelectedKeyId type should be FBlackboard::FKey, but typedefs are not supported by UHT
// 	static_assert(sizeof(uint8) == sizeof(FBlackboard::FKey), "FBlackboardKeySelector::SelectedKeyId should be of FBlackboard::FKey-compatible type.");
//
// 	// Requires BlueprintReadWrite so that blueprint creators (using MakeBlackboardKeySelector) can specify whether or not None is Allowed.
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blackboard, Meta = (Tooltip = ""))
// 		uint32 bNoneIsAllowedValue : 1;
//
// 	/** find initial selection. Called when None is not a valid option for this key selector */
// 	void InitSelection(const UBlackboardData& BlackboardAsset);
//
// public:
// 	/** find ID and class of selected key */
// 	void ResolveSelectedKey(const UBlackboardData& BlackboardAsset);
//
// 	void AllowNoneAsValue(bool bAllow) { bNoneIsAllowedValue = bAllow; }
//
// 	FORCEINLINE FBlackboard::FKey GetSelectedKeyID() const { return SelectedKeyID; }
//
// 	/** helper functions for setting basic filters */
// 	void AddObjectFilter(UObject* Owner, FName PropertyName, TSubclassOf<UObject> AllowedClass);
// 	void AddClassFilter(UObject* Owner, FName PropertyName, TSubclassOf<UClass> AllowedClass);
// 	void AddEnumFilter(UObject* Owner, FName PropertyName, UEnum* AllowedEnum);
// 	void AddNativeEnumFilter(UObject* Owner, FName PropertyName, const FString& AllowedEnumName);
// 	void AddIntFilter(UObject* Owner, FName PropertyName);
// 	void AddFloatFilter(UObject* Owner, FName PropertyName);
// 	void AddBoolFilter(UObject* Owner, FName PropertyName);
// 	void AddVectorFilter(UObject* Owner, FName PropertyName);
// 	void AddRotatorFilter(UObject* Owner, FName PropertyName);
// 	void AddStringFilter(UObject* Owner, FName PropertyName);
// 	void AddNameFilter(UObject* Owner, FName PropertyName);
//
// 	FORCEINLINE bool IsNone() const { return bNoneIsAllowedValue && SelectedKeyID == FBlackboard::InvalidKey; }
// 	FORCEINLINE bool IsSet() const { return SelectedKeyID != FBlackboard::InvalidKey; }
// 	FORCEINLINE bool NeedsResolving() const { return SelectedKeyID == FBlackboard::InvalidKey && SelectedKeyName.IsNone() == false; }
// 	FORCEINLINE void InvalidateResolvedKey() { SelectedKeyID = FBlackboard::InvalidKey; }
//
// 	friend FBlackboardDecoratorDetails;
// };

