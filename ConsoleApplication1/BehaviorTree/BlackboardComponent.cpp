// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/BlackboardComponent.h"
#include "Blackboard/TdrReflection.h"
#include "Blackboard/TdrReflectionErr.h"
#include "Blackboard/BBData.h"

UBlackboardComponent::UBlackboardComponent(UBehaviorTreeComponent* btComp, const std::string& bbName, int actorID, void* bb)
{
	mBehaviorTreeComp = btComp;
	mBBName = bbName;
	mActorID = actorID;
	bPausedNotifies = false;
	bSynchronizedKeyPopulated = false;

	mBBPointer = bb;
}


UBlackboardComponent::~UBlackboardComponent()
{
	if (mBBPointer)
	{
		if (mBBName == "BB_ServerTest")
		{
			BlackBoard::BB_ServerTest* bb = (BlackBoard::BB_ServerTest*)mBBPointer;
			delete bb;
		}
		mBBPointer = NULL;
	}
}

// void UBlackboardComponent::InitializeComponent()
// {
// 	if (BlackboardAsset)
// 	{
// 		InitializeBlackboard(*BlackboardAsset);
// 	}
// }
//
// void UBlackboardComponent::UninitializeComponent()
// {
// 	if (BlackboardAsset && BlackboardAsset->HasSynchronizedKeys())
// 	{
// 		UAISystem* AISystem = UAISystem::GetCurrentSafe(GetWorld());
// 		if (AISystem)
// 		{
// 			AISystem->UnregisterBlackboardComponent(*BlackboardAsset, *this);
// 		}
// 	}
//
// 	DestroyValues();
// 	Super::UninitializeComponent();
// }

// void UBlackboardComponent::CacheBrainComponent(UBrainComponent& BrainComponent)
// {
// 	if (&BrainComponent != BrainComp)
// 	{
// 		BrainComp = &BrainComponent;
// 	}
// }

struct FBlackboardInitializationData
{
	FBlackboard::FKey KeyID;
	uint16 DataSize;

	FBlackboardInitializationData() {}
	FBlackboardInitializationData(FBlackboard::FKey InKeyID, uint16 InDataSize) : KeyID(InKeyID)
	{
		DataSize = (InDataSize <= 2) ? InDataSize : ((InDataSize + 3) & ~3);
	}

	struct FMemorySort
	{
		inline bool operator()(const FBlackboardInitializationData& A, const FBlackboardInitializationData& B) const
		{
			return A.DataSize > B.DataSize;
		}
	};
};

// void UBlackboardComponent::InitializeParentChain(UBlackboardData* NewAsset)
// {
// 	if (NewAsset)
// 	{
// 		InitializeParentChain(NewAsset->Parent);
// 		NewAsset->UpdateKeyIDs();
// 	}
// }
//
// bool UBlackboardComponent::InitializeBlackboard(UBlackboardData& NewAsset)
// {
// 	// if we re-initialize with the same asset then there's no point
// 	// in reseting, since we'd lose all the accumulated knowledge
// 	if (&NewAsset == BlackboardAsset)
// 	{
// 		return false;
// 	}
//
// 	UAISystem* AISystem = UAISystem::GetCurrentSafe(GetWorld());
// 	if (AISystem == nullptr)
// 	{
// 		return false;
// 	}
//
// 	if (BlackboardAsset && BlackboardAsset->HasSynchronizedKeys())
// 	{
// 		AISystem->UnregisterBlackboardComponent(*BlackboardAsset, *this);
// 		DestroyValues();
// 	}
//
// 	BlackboardAsset = &NewAsset;
// 	ValueMemory.Reset();
// 	ValueOffsets.Reset();
// 	bSynchronizedKeyPopulated = false;
//
// 	bool bSuccess = true;
//
// 	if (BlackboardAsset->IsValid())
// 	{
// 		InitializeParentChain(BlackboardAsset);
//
// 		std::vector<FBlackboardInitializationData> InitList;
// 		const int32 NumKeys = BlackboardAsset->GetNumKeys();
// 		InitList.Reserve(NumKeys);
// 		ValueOffsets.AddZeroed(NumKeys);
//
// 		for (UBlackboardData* It = BlackboardAsset; It; It = It->Parent)
// 		{
// 			for (int32 KeyIndex = 0; KeyIndex < It->Keys.size(); KeyIndex++)
// 			{
// 				UBlackboardKeyType* KeyType = It->Keys[KeyIndex].KeyType;
// 				if (KeyType)
// 				{
// 					KeyType->PreInitialize(*this);
//
// 					const uint16 KeyMemory = KeyType->GetValueSize() + (KeyType->HasInstance() ? sizeof(FBlackboardInstancedKeyMemory) : 0);
// 					InitList.Add(FBlackboardInitializationData(KeyIndex + It->GetFirstKeyID(), KeyMemory));
// 				}
// 			}
// 		}
//
// 		// sort key values by memory size, so they can be packed better
// 		// it still won't protect against structures, that are internally misaligned (-> uint8, uint32)
// 		// but since all Engine level keys are good...
// 		InitList.Sort(FBlackboardInitializationData::FMemorySort());
// 		uint16 MemoryOffset = 0;
// 		for (int32 Index = 0; Index < InitList.size(); Index++)
// 		{
// 			ValueOffsets[InitList[Index].KeyID] = MemoryOffset;
// 			MemoryOffset += InitList[Index].DataSize;
// 		}
//
// 		ValueMemory.AddZeroed(MemoryOffset);
//
// 		// initialize memory
// 		KeyInstances.AddZeroed(InitList.size());
// 		for (int32 Index = 0; Index < InitList.size(); Index++)
// 		{
// 			const FBlackboardEntry* KeyData = BlackboardAsset->GetKey(InitList[Index].KeyID);
// 			KeyData->KeyType->InitializeKey(*this, InitList[Index].KeyID);
// 		}
//
// 		// naive initial synchronization with one of already instantiated blackboards using the same BB asset
// 		if (BlackboardAsset->HasSynchronizedKeys())
// 		{
// 			PopulateSynchronizedKeys();
// 		}
// 	}
// 	else
// 	{
// 		bSuccess = false;
// 		UE_LOG(LogBehaviorTree, Error, TEXT("Blackboard asset (%s) has errors and can't be used!"), *GetNameSafe(BlackboardAsset));
// 	}
//
// 	return bSuccess;
// }

// void UBlackboardComponent::DestroyValues()
// {
// 	for (UBlackboardData* It = BlackboardAsset; It; It = It->Parent)
// 	{
// 		for (int32 KeyIndex = 0; KeyIndex < It->Keys.size(); KeyIndex++)
// 		{
// 			UBlackboardKeyType* KeyType = It->Keys[KeyIndex].KeyType;
// 			if (KeyType)
// 			{
// 				const int32 UseIdx = KeyIndex + It->GetFirstKeyID();
// 				uint8* KeyMemory = GetKeyRawData(UseIdx);
// 				KeyType->WrappedFree(*this, KeyMemory);
// 			}
// 		}
// 	}
//
// 	ValueOffsets.Reset();
// 	ValueMemory.Reset();
// }
//
// void UBlackboardComponent::PopulateSynchronizedKeys()
// {
// 	assert(bSynchronizedKeyPopulated == false);
//
// 	UAISystem* AISystem = UAISystem::GetCurrentSafe(GetWorld());
// 	check(AISystem);
// 	AISystem->RegisterBlackboardComponent(*BlackboardAsset, *this);
//
// 	for (auto Iter = AISystem->CreateBlackboardDataToComponentsIterator(*BlackboardAsset); Iter; ++Iter)
// 	{
// 		UBlackboardComponent* OtherBlackboard = Iter.Value();
// 		if (OtherBlackboard != nullptr && ShouldSyncWithBlackboard(*OtherBlackboard))
// 		{
// 			for (const auto& Key : BlackboardAsset->Keys)
// 			{
// 				if (Key.bInstanceSynced)
// 				{
// 					UBlackboardData* const OtherBlackboardAsset = OtherBlackboard->GetBlackboardAsset();
// 					const int32 OtherKeyID = OtherBlackboardAsset ? OtherBlackboardAsset->GetKeyID(Key.EntryName) : FBlackboard::InvalidKey;
// 					if (OtherKeyID != FBlackboard::InvalidKey)
// 					{
// 						const FBlackboardEntry* const OtherKey = OtherBlackboard->GetBlackboardAsset()->GetKey(OtherKeyID);
// 						check(Key.EntryName == OtherKey->EntryName);
// 						check(Key.KeyType == OtherKey->KeyType);
//
// 						const uint16 DataOffset = Key.KeyType->IsInstanced() ? sizeof(FBlackboardInstancedKeyMemory) : 0;
// 						const int32 KeyID = BlackboardAsset->GetKeyID(Key.EntryName);
// 						uint8* RawData = GetKeyRawData(KeyID) + DataOffset;
// 						uint8* RawSource = OtherBlackboard->GetKeyRawData(OtherKeyID) + DataOffset;
//
// 						UBlackboardKeyType* KeyOb = Key.KeyType->IsInstanced() ? KeyInstances[KeyID] : Key.KeyType;
// 						const UBlackboardKeyType* SourceKeyOb = Key.KeyType->IsInstanced() ? OtherBlackboard->KeyInstances[OtherKeyID] : Key.KeyType;
//
// 						KeyOb->CopyValues(*this, RawData, SourceKeyOb, RawSource);
// 					}
// 				}
// 			}
// 			break;
// 		}
// 	}
//
// 	bSynchronizedKeyPopulated = true;
// }
//
// bool UBlackboardComponent::ShouldSyncWithBlackboard(UBlackboardComponent& OtherBlackboardComponent) const
// {
// 	return &OtherBlackboardComponent != this && (
// 		(BrainComp == nullptr || (BrainComp->GetAIOwner() != nullptr && BrainComp->GetAIOwner()->ShouldSyncBlackboardWith(OtherBlackboardComponent) == true))
// 		|| (OtherBlackboardComponent.BrainComp == nullptr || (OtherBlackboardComponent.BrainComp->GetAIOwner() != nullptr && OtherBlackboardComponent.BrainComp->GetAIOwner()->ShouldSyncBlackboardWith(*this) == true)));
// }

//
// UBlackboardData* UBlackboardComponent::GetBlackboardAsset() const
// {
// 	return BlackboardAsset;
// }
//
// FName UBlackboardComponent::GetKeyName(FBlackboard::FKey KeyID) const
// {
// 	return BlackboardAsset ? BlackboardAsset->GetKeyName(KeyID) : NAME_None;
// }
//
// FBlackboard::FKey UBlackboardComponent::GetKeyID(const FName& KeyName) const
// {
// 	return BlackboardAsset ? BlackboardAsset->GetKeyID(KeyName) : FBlackboard::InvalidKey;
// }
//
// TSubclassOf<UBlackboardKeyType> UBlackboardComponent::GetKeyType(FBlackboard::FKey KeyID) const
// {
// 	return BlackboardAsset ? BlackboardAsset->GetKeyType(KeyID) : NULL;
// }
//
// bool UBlackboardComponent::IsKeyInstanceSynced(FBlackboard::FKey KeyID) const
// {
// 	return BlackboardAsset ? BlackboardAsset->IsKeyInstanceSynced(KeyID) : false;
// }
//
// int32 UBlackboardComponent::GetNumKeys() const
// {
// 	return 0;// BlackboardAsset ? BlackboardAsset->GetNumKeys() : 0;
// }

FDelegateHandle UBlackboardComponent::RegisterObserver(std::string KeyID, UBTNode* NotifyOwner, FOnBlackboardChangeNotification ObserverDelegate)
{
	Observers.push_back(std::pair<std::string, FOnBlackboardChangeNotification>(KeyID, ObserverDelegate));
	return ObserverDelegate.GetHandle();
}

void UBlackboardComponent::UnregisterObserver(std::string KeyID, FDelegateHandle ObserverHandle)
{
	size_t n = Observers.size();
	for (size_t i = n - 1; i > 0; --i)
	{
		BlackboardChangeNotificationData& data = Observers[i];
		if (data.first == KeyID && data.second.GetHandle() == ObserverHandle)
		{
			Observers.erase(Observers.begin() + i);
			break;
		}
	}
}

void UBlackboardComponent::UnregisterObserversFrom(UBTNode* NotifyOwner)
{
	size_t n = Observers.size();
	for (size_t i = n - 1; i > 0; --i)
	{
		BlackboardChangeNotificationData& data = Observers[i];
		if (data.second.GetObjectPtr() == (void*)NotifyOwner)
		{
			Observers.erase(Observers.begin() + i);
		}
	}
}

void UBlackboardComponent::PauseObserverNotifications()
{
	bPausedNotifies = true;
}


void UBlackboardComponent::ResumeObserverNotifications(bool bSendQueuedObserverNotifications)
{
	bPausedNotifies = false;

	if (bSendQueuedObserverNotifications)
	{
		for (int32 UpdateIndex = 0; UpdateIndex < (int)QueuedUpdates.size(); UpdateIndex++)
		{
			NotifyObservers(QueuedUpdates[UpdateIndex]);
		}
	}

	QueuedUpdates.clear();
}

void UBlackboardComponent::NotifyObservers(const std::string& KeyID) const
{

	if (bPausedNotifies)
	{
		for (size_t i = 0; i < QueuedUpdates.size(); ++i)
		{
			if (KeyID == QueuedUpdates[i])
			{
				return;
			}
		}
		QueuedUpdates.push_back(KeyID);
	}
	else
	{
		for (size_t i = 0; i < Observers.size(); )
		{
			if (KeyID != Observers[i].first)
			{
				++i;
				continue;
			}
			FOnBlackboardChangeNotification& ObserverDelegate = Observers[i].second;
			const bool bWantsToContinueObserving = (ObserverDelegate.Execute(*this, KeyID) == EBlackboardNotificationResult_ContinueObserving);

			if (bWantsToContinueObserving == false)
			{
				Observers.erase(Observers.begin() + i);
			}
			else
			{
				++i;
			}
		}
	}
}
//
// bool UBlackboardComponent::IsCompatibleWith(UBlackboardData* TestAsset) const
// {
// 	for (UBlackboardData* It = BlackboardAsset; It; It = It->Parent)
// 	{
// 		if (It == TestAsset)
// 		{
// 			return true;
// 		}
//
// 		if (It->Keys == TestAsset->Keys)
// 		{
// 			return true;
// 		}
// 	}
//
// 	return false;
// }

// EBlackboardCompare::Type UBlackboardComponent::CompareKeyValues(TSubclassOf<UBlackboardKeyType> KeyType, FBlackboard::FKey KeyA, FBlackboard::FKey KeyB) const
// {
// 	const uint8* KeyAMemory = GetKeyRawData(KeyA) + (KeyInstances[KeyA] ? sizeof(FBlackboardInstancedKeyMemory) : 0);
// 	const uint8* KeyBMemory = GetKeyRawData(KeyB) + (KeyInstances[KeyB] ? sizeof(FBlackboardInstancedKeyMemory) : 0);
//
// 	const UBlackboardKeyType* KeyAOb = KeyInstances[KeyA] ? KeyInstances[KeyA] : KeyType->GetDefaultObject<UBlackboardKeyType>();
// 	return KeyAOb->CompareValues(*this, KeyAMemory, KeyInstances[KeyB], KeyBMemory);
// }
//
// FString UBlackboardComponent::GetDebugInfoString(EBlackboardDescription::Type Mode) const
// {
// 	FString DebugString = FString::Printf(TEXT("Blackboard (asset: %s)\n"), *GetNameSafe(BlackboardAsset));
//
// 	std::vector<FString> KeyDesc;
// 	uint8 Offset = 0;
// 	for (UBlackboardData* It = BlackboardAsset; It; It = It->Parent)
// 	{
// 		for (int32 KeyIndex = 0; KeyIndex < It->Keys.size(); KeyIndex++)
// 		{
// 			KeyDesc.Add(DescribeKeyValue(KeyIndex + Offset, Mode));
// 		}
// 		Offset += It->Keys.size();
// 	}
//
// 	KeyDesc.Sort();
// 	for (int32 KeyDescIndex = 0; KeyDescIndex < KeyDesc.size(); KeyDescIndex++)
// 	{
// 		DebugString += TEXT("  ");
// 		DebugString += KeyDesc[KeyDescIndex];
// 		DebugString += TEXT('\n');
// 	}
//
// 	if (Mode == EBlackboardDescription::Full && BlackboardAsset)
// 	{
// 		DebugString += TEXT("Observed Keys:\n");
//
// 		std::vector<uint8> ObserversKeys;
// 		if (Observers.size() > 0)
// 		{
// 			Observers.GetKeys(ObserversKeys);
//
// 			for (int32 KeyIndex = 0; KeyIndex < ObserversKeys.size(); ++KeyIndex)
// 			{
// 				const FBlackboard::FKey KeyID = ObserversKeys[KeyIndex];
// 				//@todo shouldn't be using a localized value?; GetKeyName() [10/11/2013 justin.sargent]
// 				DebugString += FString::Printf(TEXT("  %s:\n"), *BlackboardAsset->GetKeyName(KeyID).ToString());
// 			}
// 		}
// 		else
// 		{
// 			DebugString += TEXT("  NONE\n");
// 		}
// 	}
//
// 	return DebugString;
// }
//
// FString UBlackboardComponent::DescribeKeyValue(const FName& KeyName, EBlackboardDescription::Type Mode) const
// {
// 	return DescribeKeyValue(GetKeyID(KeyName), Mode);
// }
//
// FString UBlackboardComponent::DescribeKeyValue(FBlackboard::FKey KeyID, EBlackboardDescription::Type Mode) const
// {
// 	FString Description;
//
// 	const FBlackboardEntry* Key = BlackboardAsset ? BlackboardAsset->GetKey(KeyID) : NULL;
// 	if (Key)
// 	{
// 		const uint8* ValueData = GetKeyRawData(KeyID);
// 		FString ValueDesc = Key->KeyType && ValueData ? *(Key->KeyType->WrappedDescribeValue(*this, ValueData)) : TEXT("empty");
//
// 		if (Mode == EBlackboardDescription::OnlyValue)
// 		{
// 			Description = ValueDesc;
// 		}
// 		else if (Mode == EBlackboardDescription::KeyWithValue)
// 		{
// 			Description = FString::Printf(TEXT("%s: %s"), *(Key->EntryName.ToString()), *ValueDesc);
// 		}
// 		else
// 		{
// 			const FString CommonTypePrefix = UBlackboardKeyType::StaticClass()->GetName().AppendChar(TEXT('_'));
// 			const FString FullKeyType = Key->KeyType ? GetNameSafe(Key->KeyType->GetClass()) : FString();
// 			const FString DescKeyType = FullKeyType.StartsWith(CommonTypePrefix) ? FullKeyType.RightChop(CommonTypePrefix.Len()) : FullKeyType;
//
// 			Description = FString::Printf(TEXT("%s [%s]: %s"), *(Key->EntryName.ToString()), *DescKeyType, *ValueDesc);
// 		}
// 	}
//
// 	return Description;
// }

#if ENABLE_VISUAL_LOG

void UBlackboardComponent::DescribeSelfToVisLog(FVisualLogEntry* Snapshot) const
{
	FVisualLogStatusCategory Category;
	Category.Category = FString::Printf(TEXT("Blackboard (asset: %s)"), *GetNameSafe(BlackboardAsset));

	// describe only when values are initialized
	if (ValueMemory.size())
	{
		for (UBlackboardData* It = BlackboardAsset; It; It = It->Parent)
		{
			for (int32 KeyIndex = 0; KeyIndex < It->Keys.size(); KeyIndex++)
			{
				const FBlackboardEntry& Key = It->Keys[KeyIndex];

				const uint8* ValueData = GetKeyRawData(It->GetFirstKeyID() + KeyIndex);
				FString ValueDesc = Key.KeyType ? *(Key.KeyType->WrappedDescribeValue(*this, ValueData)) : TEXT("empty");

				Category.Add(Key.EntryName.ToString(), ValueDesc);
			}
		}

		Category.Data.Sort();
	}

	Snapshot->Status.Add(Category);
}

#endif

// UObject* UBlackboardComponent::GetValueAsObject(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Object>(KeyName);
// }
//
// UClass* UBlackboardComponent::GetValueAsClass(const FName& KeyName) const
// {
// 	return GetValue<UBlackboardKeyType_Class>(KeyName);
// }

uint8 UBlackboardComponent::GetValueAsEnum(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	int64_t val;
	int iRet = GetIntBBValue(val, mActorID, mBBName, KeyName);
	if (errorVal != NULL)
	{
		*errorVal = iRet;
	}
	if (iRet == Err_BB_OK)
	{
		uint8 ret = (uint8)val;
		return ret;
	}
	return 0;
}

int32 UBlackboardComponent::GetValueAsInt(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	int64_t val;
	int iRet = GetIntBBValue(val, mActorID, mBBName, KeyName);
	if (errorVal != NULL)
	{
		*errorVal = iRet;
	}
	if (iRet == Err_BB_OK)
	{
		int32 ret = (int32)val;
		return ret;
	}
	return 0;
}

float UBlackboardComponent::GetValueAsFloat(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	double val;
	int iRet = GetFloatBBValue(val, mActorID, mBBName, KeyName);
	if (errorVal != NULL)
	{
		*errorVal = iRet;
	}
	if (iRet == Err_BB_OK)
	{
		float ret = (float)val;
		return ret;
	}
	return 0;
}

bool UBlackboardComponent::GetValueAsBool(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	int64_t val;
	int iRet = GetIntBBValue(val, mActorID, mBBName, KeyName);
	if (errorVal != NULL)
	{
		*errorVal = iRet;
	}
	if (iRet == Err_BB_OK)
	{
		bool ret = (bool)val;
		return ret;
	}
	return false;
}

FString UBlackboardComponent::GetValueAsString(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	std::string val;
	int iRet = GetStrBBValue(val, mActorID, mBBName, KeyName);
	if (errorVal != NULL)
	{
		*errorVal = iRet;
	}
	if (iRet == Err_BB_OK)
	{
		return val;
	}
	return std::string();
}

FName UBlackboardComponent::GetValueAsName(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	return GetValueAsString(KeyName);
}

FVector UBlackboardComponent::GetValueAsVector(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	std::vector<double> arr;
	int iRet = GetFloatArrayBBValue(arr, mActorID, mBBName, KeyName);
	if (errorVal != NULL)
	{
		*errorVal = iRet;
	}
	if (iRet == Err_BB_OK && arr.size() == 3)
	{
		return FVector(arr[0], arr[1], arr[2]);
	}
	return FVector::InvalidVector;
}

FRotator UBlackboardComponent::GetValueAsRotator(const FName& KeyName, int* errorVal/* = NULL*/) const
{
	std::vector<double> arr;
	int iRet = GetFloatArrayBBValue(arr, mActorID, mBBName, KeyName);
	if (errorVal != NULL)
	{
		*errorVal = iRet;
	}
	if (iRet == Err_BB_OK && arr.size() == 3)
	{
		return FRotator(arr[0], arr[1], arr[2]);
	}
	return FRotator::InvalidRotator;
}
//
// void UBlackboardComponent::SetValueAsObject(const FName& KeyName, UObject* ObjectValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Object>(KeyID, ObjectValue);
// }
//
// void UBlackboardComponent::SetValueAsClass(const FName& KeyName, UClass* ClassValue)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	SetValue<UBlackboardKeyType_Class>(KeyID, ClassValue);
// }

void UBlackboardComponent::SetValueAsEnum(const FName& KeyName, uint8 EnumValue)
{
	if (GetValueAsEnum(KeyName) != EnumValue)
	{
		SetIntBBValue(mActorID, mBBName, KeyName, (int64_t)EnumValue);

		NotifyObservers(KeyName);
	}
}

void UBlackboardComponent::SetValueAsInt(const FName& KeyName, int32 IntValue)
{
	if (GetValueAsInt(KeyName) != IntValue)
	{
		SetIntBBValue(mActorID, mBBName, KeyName, IntValue);

		NotifyObservers(KeyName);
	}
}

void UBlackboardComponent::SetValueAsFloat(const FName& KeyName, float FloatValue)
{
	if (GetValueAsFloat(KeyName) != FloatValue)
	{
		SetFloatBBValue(mActorID, mBBName, KeyName, FloatValue);

		NotifyObservers(KeyName);
	}
}

void UBlackboardComponent::SetValueAsBool(const FName& KeyName, bool BoolValue)
{
	if (GetValueAsBool(KeyName) != BoolValue)
	{
		SetIntBBValue(mActorID, mBBName, KeyName, (int64_t)BoolValue);

		NotifyObservers(KeyName);
	}
}

void UBlackboardComponent::SetValueAsString(const FName& KeyName, FString StringValue)
{
	if (GetValueAsString(KeyName) != StringValue)
	{
		SetStrBBValue(mActorID, mBBName, KeyName, StringValue);

		NotifyObservers(KeyName);
	}
}

void UBlackboardComponent::SetValueAsName(const FName& KeyName, FName NameValue)
{
	if (GetValueAsName(KeyName) != NameValue)
	{
		SetStrBBValue(mActorID, mBBName, KeyName, NameValue);

		NotifyObservers(KeyName);
	}
}

void UBlackboardComponent::SetValueAsVector(const FName& KeyName, FVector VectorValue)
{
	if (GetValueAsVector(KeyName) != VectorValue)
	{
		std::vector<double> arr;
		arr.push_back(VectorValue.X);
		arr.push_back(VectorValue.Y);
		arr.push_back(VectorValue.Z);
		SetFloatArrayBBValue(mActorID, mBBName, KeyName, arr);

		NotifyObservers(KeyName);
	}
}

void UBlackboardComponent::SetValueAsRotator(const FName& KeyName, FRotator RotatorValue)
{
	if (GetValueAsRotator(KeyName) != RotatorValue)
	{
		std::vector<double> arr;
		arr.push_back(RotatorValue.Pitch);
		arr.push_back(RotatorValue.Yaw);
		arr.push_back(RotatorValue.Roll);
		SetFloatArrayBBValue(mActorID, mBBName, KeyName, arr);

		NotifyObservers(KeyName);
	}
}

bool UBlackboardComponent::IsVectorValueSet(const FName& KeyName) const
{
	FVector vec = GetValueAsVector(KeyName);
	return (vec == FVector::InvalidVector);
}

// bool UBlackboardComponent::IsVectorValueSet(FBlackboard::FKey KeyID) const
// {
// 	FVector VectorValue = GetValue<UBlackboardKeyType_Vector>(KeyID);
// 	return (VectorValue != FAISystem::InvalidLocation);
// }

// void UBlackboardComponent::ClearValue(const FName& KeyName)
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	ClearValue(KeyID);
// }
//
// void UBlackboardComponent::ClearValue(FBlackboard::FKey KeyID)
// {
// 	if (!ensureAsRuntimeWarning(BlackboardAsset != nullptr))
// 	{
// 		return;
// 	}
//
// 	const FBlackboardEntry* EntryInfo = BlackboardAsset->GetKey(KeyID);
//
// 	uint8* RawData = GetKeyRawData(KeyID);
// 	if (RawData)
// 	{
// 		const bool bHasData = (EntryInfo->KeyType->WrappedIsEmpty(*this, RawData) == false);
// 		if (bHasData)
// 		{
// 			EntryInfo->KeyType->WrappedClear(*this, RawData);
// 			NotifyObservers(KeyID);
//
// 			if (BlackboardAsset->HasSynchronizedKeys() && IsKeyInstanceSynced(KeyID))
// 			{
// 				UBlackboardKeyType* KeyOb = EntryInfo->KeyType->IsInstanced() ? KeyInstances[KeyID] : EntryInfo->KeyType;
// 				const uint16 DataOffset = EntryInfo->KeyType->IsInstanced() ? sizeof(FBlackboardInstancedKeyMemory) : 0;
// 				uint8* InstancedRawData = RawData + DataOffset;
//
// 				// grab the value set and apply the same to synchronized keys
// 				// to avoid virtual function call overhead
// 				UAISystem* AISystem = UAISystem::GetCurrentSafe(GetWorld());
// 				for (auto Iter = AISystem->CreateBlackboardDataToComponentsIterator(*BlackboardAsset); Iter; ++Iter)
// 				{
// 					UBlackboardComponent* OtherBlackboard = Iter.Value();
// 					if (OtherBlackboard != nullptr && ShouldSyncWithBlackboard(*OtherBlackboard))
// 					{
// 						UBlackboardData* const OtherBlackboardAsset = OtherBlackboard->GetBlackboardAsset();
// 						const int32 OtherKeyID = OtherBlackboardAsset ? OtherBlackboardAsset->GetKeyID(EntryInfo->EntryName) : FBlackboard::InvalidKey;
// 						if (OtherKeyID != FBlackboard::InvalidKey)
// 						{
// 							const FBlackboardEntry* OtherEntryInfo = OtherBlackboard->BlackboardAsset->GetKey(OtherKeyID);
// 							UBlackboardKeyType* OtherKeyOb = EntryInfo->KeyType->IsInstanced() ? OtherBlackboard->KeyInstances[OtherKeyID] : EntryInfo->KeyType;
// 							uint8* OtherRawData = OtherBlackboard->GetKeyRawData(OtherKeyID) + DataOffset;
//
// 							OtherKeyOb->CopyValues(*OtherBlackboard, OtherRawData, KeyOb, InstancedRawData);
// 							OtherBlackboard->NotifyObservers(OtherKeyID);
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// }
//
// bool UBlackboardComponent::GetLocationFromEntry(const FName& KeyName, FVector& ResultLocation) const
// {
// 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// 	return GetLocationFromEntry(KeyID, ResultLocation);
// }
//
// bool UBlackboardComponent::GetLocationFromEntry(FBlackboard::FKey KeyID, FVector& ResultLocation) const
// {
// 	if (BlackboardAsset && ValueOffsets.IsValidIndex(KeyID))
// 	{
// 		const FBlackboardEntry* EntryInfo = BlackboardAsset->GetKey(KeyID);
// 		if (EntryInfo && EntryInfo->KeyType)
// 		{
// 			const uint8* ValueData = ValueMemory.GetData() + ValueOffsets[KeyID];
// 			return EntryInfo->KeyType->WrappedGetLocation(*this, ValueData, ResultLocation);
// 		}
// 	}
//
// 	return false;
// }
//
// bool UBlackboardComponent::GetRotationFromEntry(const FName& KeyName, FRotator& ResultRotation) const
// {
// // 	const FBlackboard::FKey KeyID = GetKeyID(KeyName);
// // 	return GetRotationFromEntry(KeyID, ResultRotation);
// 	return false;
// }
//
// bool UBlackboardComponent::GetRotationFromEntry(FBlackboard::FKey KeyID, FRotator& ResultRotation) const
// {
// // 	if (BlackboardAsset && ValueOffsets.IsValidIndex(KeyID))
// // 	{
// // 		const FBlackboardEntry* EntryInfo = BlackboardAsset->GetKey(KeyID);
// // 		if (EntryInfo && EntryInfo->KeyType)
// // 		{
// // 			const uint8* ValueData = ValueMemory.GetData() + ValueOffsets[KeyID];
// // 			return EntryInfo->KeyType->WrappedGetRotation(*this, ValueData, ResultRotation);
// // 		}
// // 	}
//
// 	return false;
// }
