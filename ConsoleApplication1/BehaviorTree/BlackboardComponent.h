#pragma once

#include "BehaviorTreeTypes.h"
#include "Core/MyDelegate.h"

class BlackboardData;

namespace EBlackboardDescription
{
enum Type
{
	OnlyValue,
	KeyWithValue,
	DetailedKeyWithValue,
	Full,
};
}

class UBlackboardComponent
{
public:
	UBlackboardComponent(UBehaviorTreeComponent* btComp, const std::string& bbName, int actorID, void* bb);
	virtual ~UBlackboardComponent();

	/** BEGIN UActorComponents */
// 	virtual void InitializeComponent();
// 	virtual void UninitializeComponent();
	/** END UActorComponents */

	/** @return number of entries in data asset */
	//int32 GetNumKeys() const;

	/** register observer for blackboard key */
	FDelegateHandle RegisterObserver(std::string KeyID, UBTNode* NotifyOwner, FOnBlackboardChangeNotification ObserverDelegate);

	/** unregister observer from blackboard key */
	void UnregisterObserver(std::string KeyID, FDelegateHandle ObserverHandle);

	/** unregister all observers associated with given owner */
	void UnregisterObserversFrom(UBTNode* NotifyOwner);

	/** pause observer change notifications, any new ones will be added to a queue */
	void PauseObserverNotifications();

	/** resume observer change notifications and, optionally, process the queued observation list */
	void ResumeObserverNotifications(bool bSendQueuedObserverNotifications);

	/** @return blackboard data asset */
	//UBlackboardData* GetBlackboardAsset() const;

	/** caches UBrainComponent pointer to be used in communication */
	//void CacheBrainComponent(UBrainComponent& BrainComponent);

	/** setup component for using given blackboard asset */
	//bool InitializeBlackboard(UBlackboardData& NewAsset);

	/** @return true if component can be used with specified blackboard asset */
	//bool IsCompatibleWith(UBlackboardData* TestAsset) const;

// 	UFUNCTION(BlueprintCallable, Category="AI|Components|Blackboard")
// 	UObject* GetValueAsObject(const FName& KeyName) const;
//
// 	UFUNCTION(BlueprintCallable, Category="AI|Components|Blackboard")
// 	UClass* GetValueAsClass(const FName& KeyName) const;


	uint8 GetValueAsEnum(const FName& KeyName, int* errorVal = NULL) const;


	int32 GetValueAsInt(const FName& KeyName, int* errorVal = NULL) const;


	float GetValueAsFloat(const FName& KeyName, int* errorVal = NULL) const;


	bool GetValueAsBool(const FName& KeyName, int* errorVal = NULL) const;


	FString GetValueAsString(const FName& KeyName, int* errorVal = NULL) const;


	FName GetValueAsName(const FName& KeyName, int* errorVal = NULL) const;


	FVector GetValueAsVector(const FName& KeyName, int* errorVal = NULL) const;


	FRotator GetValueAsRotator(const FName& KeyName, int* errorVal = NULL) const;


	//void SetValueAsObject(const FName& KeyName, UObject* ObjectValue);


	//void SetValueAsClass(const FName& KeyName, UClass* ClassValue);


	void SetValueAsEnum(const FName& KeyName, uint8 EnumValue);


	void SetValueAsInt(const FName& KeyName, int32 IntValue);


	void SetValueAsFloat(const FName& KeyName, float FloatValue);


	void SetValueAsBool(const FName& KeyName, bool BoolValue);


	void SetValueAsString(const FName& KeyName, FString StringValue);


	void SetValueAsName(const FName& KeyName, FName NameValue);


	void SetValueAsVector(const FName& KeyName, FVector VectorValue);

	void SetValueAsRotator(const FName& KeyName, FRotator VectorValue);

	bool IsVectorValueSet(const FName& KeyName) const;
	//bool IsVectorValueSet(FBlackboard::FKey KeyID) const;

	/** return false if call failed (most probably no such entry in BB) */
//
// 	bool GetLocationFromEntry(const FName& KeyName, FVector& ResultLocation) const;
// 	bool GetLocationFromEntry(FBlackboard::FKey KeyID, FVector& ResultLocation) const;
//
// 	/** return false if call failed (most probably no such entry in BB) */
//
// 	bool GetRotationFromEntry(const FName& KeyName, FRotator& ResultRotation) const;
// 	bool GetRotationFromEntry(FBlackboard::FKey KeyID, FRotator& ResultRotation) const;
//
// 	/** compares blackboard's values under specified keys */
// 	EBlackboardCompare::Type CompareKeyValues(TSubclassOf<UBlackboardKeyType> KeyType, FBlackboard::FKey KeyA, FBlackboard::FKey KeyB) const;
//
// 	FString GetDebugInfoString(EBlackboardDescription::Type Mode) const;
//
// 	/** get description of value under given key */
// 	FString DescribeKeyValue(const FName& KeyName, EBlackboardDescription::Type Mode) const;
// 	FString DescribeKeyValue(FBlackboard::FKey KeyID, EBlackboardDescription::Type Mode) const;
//
// #if ENABLE_VISUAL_LOG
// 	/** prepare blackboard snapshot for logs */
// 	virtual void DescribeSelfToVisLog(struct FVisualLogEntry* Snapshot) const;
// #endif
	inline UBehaviorTreeComponent* GetBrainComponent() const { return mBehaviorTreeComp; }

	inline const FString& GetBBName() const { return mBBName; }

	inline int GetActorID() const { return mActorID; }
protected:
	//std::string mBBXmlPath;
	std::string mBBName;
	int		mActorID;
	void* mBBPointer;
	UBehaviorTreeComponent* mBehaviorTreeComp;

protected:
	/** observers registered for blackboard keys */
	typedef std::pair<std::string, FOnBlackboardChangeNotification> BlackboardChangeNotificationData;
	mutable vector<BlackboardChangeNotificationData> Observers;

	/** queued key change notification, will be processed on ResumeUpdates call */
	mutable vector<std::string> QueuedUpdates;

	/** set when observation notifies are paused and shouldn't be passed to observers */
	uint32 bPausedNotifies : 1;

	/** reset to false every time a new BB asset is assigned to this component */
	uint32 bSynchronizedKeyPopulated : 1;

	/** notifies behavior tree decorators about change in blackboard */
	void NotifyObservers(const std::string& KeyID) const;

// 	/** initializes parent chain in asset */
// 	void InitializeParentChain(UBlackboardData* NewAsset);

// 	/** destroy allocated values */
// 	void DestroyValues();
//
// 	/** populates BB's synchronized entries */
// 	void PopulateSynchronizedKeys();
//
// 	bool ShouldSyncWithBlackboard(UBlackboardComponent& OtherBlackboardComponent) const;
//
// 	friend UBlackboardKeyType;
};

//////////////////////////////////////////////////////////////////////////
// Inlines
