#include "Manager/InputManager.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Info/InputInfo.h"
#include "Info/MappableInfo.h"
#include "Input/MappingBehavior.h"
#include "Input/ProjectMappableKeySetting.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/ConfigManager.h"
#include "PlayerMappableKeySettings.h"
#include "ProjectGameInstance.h"

void UInputManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UConfigManager>();

	const UConfigManager* ConfigManager = GetGameInstance()->GetSubsystem<UConfigManager>();
	CachedMappableInfo = ConfigManager ? ConfigManager->GetMappableInfo() : nullptr;
	CachedInputInfo = ConfigManager ? ConfigManager->GetInputInfo() : nullptr;
	LoadedMappableKeySetting = nullptr;
	CachedEnhancedInputLocalPlayerSubsystem.Reset();
	RuntimeMappedKeyIndexMap.Empty();
}

bool UInputManager::InitializeMappableByPlayerController(APlayerController* InPlayerController)
{
	if (!InPlayerController) return false;

	ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer();
	if (!LocalPlayer) return false;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!EnhancedInputLocalPlayerSubsystem || !CachedInputInfo) return false;

	CachedEnhancedInputLocalPlayerSubsystem = EnhancedInputLocalPlayerSubsystem;

	// 重新初始化时，先移除旧的运行时 IMC，避免重复注册
	for (const TTuple<FGameplayTag, TObjectPtr<UInputMappingContext>>& Pair : IMCTagToMappedInputMappingContext)
	{
		if (Pair.Value)
			EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(Pair.Value);
	}

	IMCTagToMappedInputMappingContext.Empty();
	RuntimeMappedKeyIndexMap.Empty();

	const FString SaveSlotName = UProjectGameInstance::GetSaveSlotName(EProjectSaveSlotName::MappableKey);
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		// 有存档时直接加载玩家保存的改键结果
		LoadedMappableKeySetting = Cast<UProjectMappableKeySetting>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (!LoadedMappableKeySetting) return false;
	}
	else
	{
		// 首次进入游戏时，用默认 IMC 生成一份初始改键存档
		LoadedMappableKeySetting = Cast<UProjectMappableKeySetting>(UGameplayStatics::CreateSaveGameObject(UProjectMappableKeySetting::StaticClass()));
		if (!LoadedMappableKeySetting) return false;
		if (!CreateMappableKeySlot()) return false;
	}

	return RebuildRuntimeInputContextsFromSaveData();
}

bool UInputManager::GetCurrentKeysByGroupTag(
	const FGameplayTag GroupTag,
	FInputChord& OutPrimaryInputChord,
	FInputChord& OutSecondaryInputChord) const
{
	OutPrimaryInputChord = FInputChord();
	OutSecondaryInputChord = FInputChord();

	const bool bHasPrimaryKey = GetCurrentPrimaryKeyByGroupTag(GroupTag, OutPrimaryInputChord);
	const bool bHasSecondaryKey = GetCurrentSecondaryKeyByGroupTag(GroupTag, OutSecondaryInputChord);
	return bHasPrimaryKey || bHasSecondaryKey;
}

bool UInputManager::GetMappedKeysDataByGroupTag(const FGameplayTag GroupTag, FGroupTagMappedKeys& OutMappedKeys) const
{
	OutMappedKeys = FGroupTagMappedKeys();
	OutMappedKeys.GroupTag = GroupTag;
	OutMappedKeys.bHasPrimaryKey = GetCurrentPrimaryKeyByGroupTag(GroupTag, OutMappedKeys.PrimaryInputChord);
	OutMappedKeys.bHasSecondaryKey = GetCurrentSecondaryKeyByGroupTag(GroupTag, OutMappedKeys.SecondaryInputChord);
	return OutMappedKeys.bHasPrimaryKey || OutMappedKeys.bHasSecondaryKey;
}

bool UInputManager::GetCurrentPrimaryKeyByGroupTag(const FGameplayTag GroupTag, FInputChord& OutInputChord) const
{
	return GetCurrentKeyByGroupTagWithSlot(GroupTag, EPlayerMappableKeySlot::First, OutInputChord);
}

bool UInputManager::GetCurrentSecondaryKeyByGroupTag(const FGameplayTag GroupTag, FInputChord& OutInputChord) const
{
	return GetCurrentKeyByGroupTagWithSlot(GroupTag, EPlayerMappableKeySlot::Second, OutInputChord);
}

bool UInputManager::GetCurrentKeyByGroupTagWithSlot(
	const FGameplayTag GroupTag,
	const EPlayerMappableKeySlot InSlot,
	FInputChord& OutInputChord) const
{
	OutInputChord = FInputChord();

	const FMappableActionGroup* MappableGroup = GetMappableGroupByTag(GroupTag);
	if (!MappableGroup) return false;

	for (const UMappingBehavior* MappableBehavior : MappableGroup->MappableBehaviors)
	{
		if (GetCurrentKeyByBehavior(MappableBehavior, InSlot, OutInputChord))
			return true;
	}

	return false;
}

bool UInputManager::RemapKeyByGroupTag(const FGameplayTag GroupTag, const FInputChord& SelectedInputChord)
{
	return RemapPrimaryKeyByGroupTag(GroupTag, SelectedInputChord);
}

bool UInputManager::RemapPrimaryKeyByGroupTag(const FGameplayTag GroupTag, const FInputChord& SelectedInputChord)
{
	return RemapKeyByGroupTagWithSlot(GroupTag, EPlayerMappableKeySlot::First, SelectedInputChord);
}

bool UInputManager::RemapSecondaryKeyByGroupTag(const FGameplayTag GroupTag, const FInputChord& SelectedInputChord)
{
	bool bHasClearedDuplicateSlot = false;
	EPlayerMappableKeySlot ClearedSlot = EPlayerMappableKeySlot::Unspecified;
	return RemapKeyByGroupTagWithSlotAndOutputClearedSlot(GroupTag, EPlayerMappableKeySlot::Second, SelectedInputChord, bHasClearedDuplicateSlot, ClearedSlot);
}

bool UInputManager::RemapKeyByGroupTagOneSlot(const FGameplayTag GroupTag, const FInputChord& SelectedInputChord)
{
	return RemapKeyByGroupTagWithSlot(GroupTag, EPlayerMappableKeySlot::First, SelectedInputChord);
}

bool UInputManager::RemapKeyByGroupTagWithSlot(
	const FGameplayTag GroupTag,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord)
{
	const FMappableActionGroup* MappableGroup = GetMappableGroupByTag(GroupTag);
	if (!MappableGroup) return false;

	bool bHasAnyTarget = false;
	bool bAllSucceeded = true;

	for (const UMappingBehavior* MappableBehavior : MappableGroup->MappableBehaviors)
	{
		if (!MappableBehavior) continue;
		if (InSlot == EPlayerMappableKeySlot::Second && !MappableBehavior->bSupportSecondarySlot) continue;

		bHasAnyTarget = true;
		if (!RemapKeyByBehavior(MappableBehavior, InSlot, SelectedInputChord))
			bAllSucceeded = false;
	}

	if (!bHasAnyTarget) return false;
	return bAllSucceeded;
}

bool UInputManager::RemapKeyByGroupTagWithSlotAndOutputClearedSlot(
	const FGameplayTag GroupTag,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord,
	bool& bOutHasClearedDuplicateSlot,
	EPlayerMappableKeySlot& OutClearedSlot)
{
	bOutHasClearedDuplicateSlot = false;
	OutClearedSlot = EPlayerMappableKeySlot::Unspecified;

	const FMappableActionGroup* MappableGroup = GetMappableGroupByTag(GroupTag);
	if (!MappableGroup) return false;

	bool bHasAnyTarget = false;
	bool bAllSucceeded = true;

	for (const UMappingBehavior* MappableBehavior : MappableGroup->MappableBehaviors)
	{
		if (!MappableBehavior) continue;
		if (InSlot == EPlayerMappableKeySlot::Second && !MappableBehavior->bSupportSecondarySlot) continue;

		bHasAnyTarget = true;

		bool bHasClearedBehaviorDuplicateSlot = false;
		EPlayerMappableKeySlot ClearedBehaviorSlot = EPlayerMappableKeySlot::Unspecified;
		if (!RemapKeyByBehaviorAndOutputClearedSlot(MappableBehavior, InSlot, SelectedInputChord, bHasClearedBehaviorDuplicateSlot, ClearedBehaviorSlot))
		{
			bAllSucceeded = false;
			continue;
		}

		if (bHasClearedBehaviorDuplicateSlot)
		{
			bOutHasClearedDuplicateSlot = true;
			OutClearedSlot = ClearedBehaviorSlot;
		}
	}

	if (!bHasAnyTarget) return false;
	return bAllSucceeded;
}

bool UInputManager::ClearKeyByGroupTagWithSlot(const FGameplayTag GroupTag, const EPlayerMappableKeySlot InSlot)
{
	const FMappableActionGroup* MappableGroup = GetMappableGroupByTag(GroupTag);
	if (!MappableGroup) return false;

	bool bHasAnyTarget = false;
	bool bAllSucceeded = true;

	for (const UMappingBehavior* MappableBehavior : MappableGroup->MappableBehaviors)
	{
		if (!MappableBehavior) continue;
		if (InSlot == EPlayerMappableKeySlot::Second && !MappableBehavior->bSupportSecondarySlot) continue;

		bHasAnyTarget = true;
		if (!ClearKeyByBehavior(MappableBehavior, InSlot))
			bAllSucceeded = false;
	}

	if (!bHasAnyTarget) return false;
	return bAllSucceeded;
}

bool UInputManager::ActivateMappedInputMappingContextByTag(const FGameplayTag IMCTag, const int32 Priority)
{
	if (!CachedEnhancedInputLocalPlayerSubsystem.IsValid() || !IMCTag.IsValid()) return false;

	const TObjectPtr<UInputMappingContext>* FoundInputMappingContext = IMCTagToMappedInputMappingContext.Find(IMCTag);
	if (!FoundInputMappingContext || !(*FoundInputMappingContext)) return false;

	CachedEnhancedInputLocalPlayerSubsystem->AddMappingContext(FoundInputMappingContext->Get(), Priority);
	return true;
}

bool UInputManager::DeactivateMappedInputMappingContextByTag(const FGameplayTag IMCTag)
{
	if (!CachedEnhancedInputLocalPlayerSubsystem.IsValid() || !IMCTag.IsValid()) return false;

	const TObjectPtr<UInputMappingContext>* FoundInputMappingContext = IMCTagToMappedInputMappingContext.Find(IMCTag);
	if (!FoundInputMappingContext || !(*FoundInputMappingContext)) return false;

	CachedEnhancedInputLocalPlayerSubsystem->RemoveMappingContext(FoundInputMappingContext->Get());
	return true;
}

const FMappableActionGroup* UInputManager::GetMappableGroupByTag(const FGameplayTag GroupTag) const
{
	if (!CachedMappableInfo || !GroupTag.IsValid()) return nullptr;

	for (const FMappableActionGroup& MappableGroup : CachedMappableInfo->MappableActionGroups)
	{
		if (MappableGroup.GroupTag == GroupTag)
			return &MappableGroup;
	}

	return nullptr;
}

const UMappingBehavior* UInputManager::FindBehaviorByNameAndIMCTag(const FName BehaviorName, const FGameplayTag IMCTag) const
{
	if (!CachedMappableInfo || BehaviorName.IsNone() || !IMCTag.IsValid()) return nullptr;

	for (const FMappableActionGroup& MappableGroup : CachedMappableInfo->MappableActionGroups)
	{
		for (const UMappingBehavior* MappableBehavior : MappableGroup.MappableBehaviors)
		{
			if (!MappableBehavior) continue;
			if (MappableBehavior->BehaviorName == BehaviorName && MappableBehavior->IMCTag == IMCTag)
				return MappableBehavior;
		}
	}

	return nullptr;
}

bool UInputManager::GetCurrentKeyByBehavior(
	const UMappingBehavior* MappableBehavior,
	const EPlayerMappableKeySlot InSlot,
	FInputChord& OutInputChord) const
{
	OutInputChord = FInputChord();

	if (!MappableBehavior || MappableBehavior->BehaviorName.IsNone()) return false;
	if (InSlot == EPlayerMappableKeySlot::Second && !MappableBehavior->bSupportSecondarySlot) return false;

	FMapPlayerOperationArgs OperationArgs;
	FGameplayTag IMCTag;
	if (!HandleOperationArgsByBehavior(MappableBehavior, InSlot, OperationArgs, IMCTag)) return false;

	const TObjectPtr<UInputMappingContext>* FoundInputMappingContext = IMCTagToMappedInputMappingContext.Find(IMCTag);
	if (!FoundInputMappingContext || !(*FoundInputMappingContext)) return false;

	const FName RuntimeMappingName = BuildRuntimeMappingNameBySlot(OperationArgs.BehaviorName, OperationArgs.Slot);
	const int32 MappingIndex = FindRuntimeMappingIndex(IMCTag, RuntimeMappingName);
	if (MappingIndex == INDEX_NONE) return false;

	const FEnhancedActionKeyMapping& InputMapping = FoundInputMappingContext->Get()->GetMappings()[MappingIndex];
	OutInputChord.Key = InputMapping.Key;
	HandleChordFlagsFromMapping(InputMapping, OutInputChord);
	return OutInputChord.Key.IsValid();
}

bool UInputManager::RemapKeyByBehavior(
	const UMappingBehavior* MappableBehavior,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord)
{
	bool bHasClearedDuplicateSlot = false;
	EPlayerMappableKeySlot ClearedSlot = EPlayerMappableKeySlot::Unspecified;
	return RemapKeyByBehaviorInternal(MappableBehavior, InSlot, SelectedInputChord, false, bHasClearedDuplicateSlot, ClearedSlot);
}

bool UInputManager::RemapKeyByBehaviorAndOutputClearedSlot(
	const UMappingBehavior* MappableBehavior,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord,
	bool& bOutHasClearedDuplicateSlot,
	EPlayerMappableKeySlot& OutClearedSlot)
{
	return RemapKeyByBehaviorInternal(MappableBehavior, InSlot, SelectedInputChord, true, bOutHasClearedDuplicateSlot, OutClearedSlot);
}

bool UInputManager::RemapKeyByBehaviorInternal(
	const UMappingBehavior* MappableBehavior,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord,
	const bool bHandleDuplicate,
	bool& bOutHasClearedDuplicateSlot,
	EPlayerMappableKeySlot& OutClearedSlot)
{
	bOutHasClearedDuplicateSlot = false;
	OutClearedSlot = EPlayerMappableKeySlot::Unspecified;

	if (!MappableBehavior || MappableBehavior->BehaviorName.IsNone()) return false;
	if (InSlot == EPlayerMappableKeySlot::Second && !MappableBehavior->bSupportSecondarySlot) return false;
	if (!SelectedInputChord.Key.IsValid()) return false;

	FMapPlayerOperationArgs OperationArgs;
	FGameplayTag IMCTag;
	if (!HandleOperationArgsByBehavior(MappableBehavior, InSlot, OperationArgs, IMCTag))
	{
		OperationArgs.BehaviorName = MappableBehavior->BehaviorName;
		OperationArgs.Slot = InSlot;
		IMCTag = MappableBehavior->IMCTag;
	}

	if (!CachedInputInfo || !IMCTag.IsValid()) return false;

	if (bHandleDuplicate)
	{
		// 双槽位改键时，先清理同一行为中与新输入完全重复的另一槽位
		bool bHasChanged = false;
		if (!ClearDuplicateKeyByBehaviorPure(MappableBehavior, InSlot, SelectedInputChord, bHasChanged)) return false;
		if (bHasChanged)
		{
			bOutHasClearedDuplicateSlot = true;
			OutClearedSlot = InSlot == EPlayerMappableKeySlot::First ? EPlayerMappableKeySlot::Second : EPlayerMappableKeySlot::First;
		}
	}

	OperationArgs.NewKey = SelectedInputChord.Key;
	OperationArgs.bAlt = SelectedInputChord.bAlt;
	OperationArgs.bControl = SelectedInputChord.bCtrl;
	OperationArgs.bShift = SelectedInputChord.bShift;

	if (!SaveMapPlayerOperationArgs(IMCTag, OperationArgs)) return false;
	if (!RebuildRuntimeInputContextByTag(IMCTag)) return false;

	RefreshControlMappings();
	return true;
}

bool UInputManager::ClearKeyByBehavior(const UMappingBehavior* MappableBehavior, const EPlayerMappableKeySlot InSlot)
{
	if (!MappableBehavior || MappableBehavior->BehaviorName.IsNone()) return false;
	if (InSlot == EPlayerMappableKeySlot::Second && !MappableBehavior->bSupportSecondarySlot) return false;

	FMapPlayerOperationArgs OperationArgs;
	FGameplayTag IMCTag;
	if (!HandleOperationArgsByBehavior(MappableBehavior, InSlot, OperationArgs, IMCTag))
	{
		OperationArgs.BehaviorName = MappableBehavior->BehaviorName;
		OperationArgs.Slot = InSlot;
		IMCTag = MappableBehavior->IMCTag;
	}

	if (!IMCTag.IsValid()) return false;

	ClearOperationArgs(OperationArgs);
	if (!SaveMapPlayerOperationArgs(IMCTag, OperationArgs)) return false;
	if (!RebuildRuntimeInputContextByTag(IMCTag)) return false;

	RefreshControlMappings();
	return true;
}

bool UInputManager::CreateMappableKeySlot()
{
	if (!CachedInputInfo || !LoadedMappableKeySetting) return false;

	// 用默认 IMC 中的可映射按键生成初始存档
	LoadedMappableKeySetting->TaggedOperationArgsArray.Empty();
	TMap<FGameplayTag, TMap<FName, int32>> IMCTagToMappingNameCount;

	for (const TTuple<FGameplayTag, FInputMappingContextWithPriority>& Pair : CachedInputInfo->IMCTagToMappableInputMappingContext)
	{
		const UInputMappingContext* InputMappingContext = Pair.Value.InputMappingContext;
		if (!InputMappingContext) continue;

		TMap<FName, int32>& MappingNameToCount = IMCTagToMappingNameCount.FindOrAdd(Pair.Key);
		const TArray<FEnhancedActionKeyMapping>& Mappings = InputMappingContext->GetMappings();

		for (const FEnhancedActionKeyMapping& InputMapping : Mappings)
		{
			const UPlayerMappableKeySettings* MappableKeySettings = InputMapping.GetPlayerMappableKeySettings();
			if (!MappableKeySettings) continue;

			const FName BaseMappingName = MappableKeySettings->GetMappingName();
			const int32 OccurrenceIndex = MappingNameToCount.FindRef(BaseMappingName);
			MappingNameToCount.Add(BaseMappingName, OccurrenceIndex + 1);

			const EPlayerMappableKeySlot Slot = static_cast<EPlayerMappableKeySlot>(OccurrenceIndex);
			if (Slot != EPlayerMappableKeySlot::First && Slot != EPlayerMappableKeySlot::Second) continue;
			if (BuildRuntimeMappingNameBySlot(BaseMappingName, Slot).IsNone()) continue;

			FMapPlayerOperationArgs DefaultOperationArgs;
			DefaultOperationArgs.BehaviorName = BaseMappingName;
			DefaultOperationArgs.Slot = Slot;
			DefaultOperationArgs.NewKey = InputMapping.Key;

			FInputChord DefaultInputChord;
			HandleChordFlagsFromMapping(InputMapping, DefaultInputChord);
			DefaultOperationArgs.bAlt = DefaultInputChord.bAlt;
			DefaultOperationArgs.bControl = DefaultInputChord.bCtrl;
			DefaultOperationArgs.bShift = DefaultInputChord.bShift;

			FTaggedMapPlayerOperationArgs* TaggedOperationArgs = FindOrAddTaggedOperationArgs(Pair.Key);
			if (!TaggedOperationArgs) continue;
			TaggedOperationArgs->OperationArgsArray.Add(DefaultOperationArgs);
		}
	}

	const FString SaveSlotName = UProjectGameInstance::GetSaveSlotName(EProjectSaveSlotName::MappableKey);
	return UGameplayStatics::SaveGameToSlot(LoadedMappableKeySetting, SaveSlotName, 0);
}

bool UInputManager::RebuildRuntimeInputContextsFromSaveData()
{
	if (!CachedInputInfo || !LoadedMappableKeySetting || !CachedEnhancedInputLocalPlayerSubsystem.IsValid()) return false;

	RuntimeMappedKeyIndexMap.Empty();

	for (const FTaggedMapPlayerOperationArgs& TaggedOperationArgs : LoadedMappableKeySetting->TaggedOperationArgsArray)
	{
		if (!TaggedOperationArgs.IMCTag.IsValid()) continue;
		if (!RebuildRuntimeInputContextByTag(TaggedOperationArgs.IMCTag)) return false;
	}

	return true;
}

bool UInputManager::RebuildRuntimeInputContextByTag(const FGameplayTag& IMCTag)
{
	if (!CachedInputInfo || !LoadedMappableKeySetting || !IMCTag.IsValid()) return false;

	UInputMappingContext* RuntimeInputMappingContext = nullptr;
	if (TObjectPtr<UInputMappingContext>* FoundRuntimeInputMappingContext = IMCTagToMappedInputMappingContext.Find(IMCTag))
		RuntimeInputMappingContext = FoundRuntimeInputMappingContext->Get();

	if (!RuntimeInputMappingContext)
	{
		RuntimeInputMappingContext = NewObject<UInputMappingContext>(this);
		if (!RuntimeInputMappingContext) return false;
		IMCTagToMappedInputMappingContext.Add(IMCTag, RuntimeInputMappingContext);
	}
	else
	{
		RuntimeInputMappingContext->UnmapAll();
	}

	RuntimeMappedKeyIndexMap.FindOrAdd(IMCTag).Empty();

	// 先按存档中的自定义键位重建可映射部分
	if (const FTaggedMapPlayerOperationArgs* TaggedOperationArgs = FindTaggedOperationArgs(IMCTag))
	{
		for (const FMapPlayerOperationArgs& MapPlayerOperationArgs : TaggedOperationArgs->OperationArgsArray)
		{
			if (!MapPlayerOperationArgs.NewKey.IsValid()) continue;
			if (!ApplyMappedInputContextOperation(RuntimeInputMappingContext, IMCTag, MapPlayerOperationArgs)) return false;
		}
	}

	// 再把默认 IMC 中不可映射的部分拷贝回来
	if (const FInputMappingContextWithPriority* DefaultInputMappingContextWithPriority = CachedInputInfo->IMCTagToMappableInputMappingContext.Find(IMCTag))
	{
		const UInputMappingContext* DefaultInputMappingContext = DefaultInputMappingContextWithPriority->InputMappingContext;
		if (!DefaultInputMappingContext) return true;

		for (const FEnhancedActionKeyMapping& DefaultInputMapping : DefaultInputMappingContext->GetMappings())
		{
			if (DefaultInputMapping.GetPlayerMappableKeySettings()) continue;
			if (!DefaultInputMapping.Action) continue;

			FEnhancedActionKeyMapping& NewInputMapping = RuntimeInputMappingContext->MapKey(DefaultInputMapping.Action, DefaultInputMapping.Key);
			NewInputMapping.Modifiers.Empty();
			NewInputMapping.Triggers.Empty();

			for (UInputModifier* InputModifier : DefaultInputMapping.Modifiers)
			{
				if (!InputModifier) continue;
				NewInputMapping.Modifiers.Add(DuplicateObject(InputModifier, RuntimeInputMappingContext));
			}

			for (UInputTrigger* InputTrigger : DefaultInputMapping.Triggers)
			{
				if (!InputTrigger) continue;
				NewInputMapping.Triggers.Add(DuplicateObject(InputTrigger, RuntimeInputMappingContext));
			}
		}
	}

	return true;
}

FTaggedMapPlayerOperationArgs* UInputManager::FindOrAddTaggedOperationArgs(const FGameplayTag& IMCTag)
{
	if (!LoadedMappableKeySetting || !IMCTag.IsValid()) return nullptr;

	for (FTaggedMapPlayerOperationArgs& TaggedOperationArgs : LoadedMappableKeySetting->TaggedOperationArgsArray)
	{
		if (TaggedOperationArgs.IMCTag == IMCTag)
			return &TaggedOperationArgs;
	}

	FTaggedMapPlayerOperationArgs NewTaggedOperationArgs;
	NewTaggedOperationArgs.IMCTag = IMCTag;
	return &LoadedMappableKeySetting->TaggedOperationArgsArray.Add_GetRef(NewTaggedOperationArgs);
}

const FTaggedMapPlayerOperationArgs* UInputManager::FindTaggedOperationArgs(const FGameplayTag& IMCTag) const
{
	if (!LoadedMappableKeySetting || !IMCTag.IsValid()) return nullptr;

	for (const FTaggedMapPlayerOperationArgs& TaggedOperationArgs : LoadedMappableKeySetting->TaggedOperationArgsArray)
	{
		if (TaggedOperationArgs.IMCTag == IMCTag)
			return &TaggedOperationArgs;
	}

	return nullptr;
}

bool UInputManager::HandleOperationArgsByBehavior(
	const UMappingBehavior* MappableBehavior,
	const EPlayerMappableKeySlot InSlot,
	FMapPlayerOperationArgs& OutOperationArgs,
	FGameplayTag& OutIMCTag) const
{
	if (!LoadedMappableKeySetting || !MappableBehavior || MappableBehavior->BehaviorName.IsNone()) return false;

	for (const FTaggedMapPlayerOperationArgs& TaggedOperationArgs : LoadedMappableKeySetting->TaggedOperationArgsArray)
	{
		if (TaggedOperationArgs.IMCTag != MappableBehavior->IMCTag) continue;

		for (const FMapPlayerOperationArgs& SavedOperationArgs : TaggedOperationArgs.OperationArgsArray)
		{
			if (SavedOperationArgs.BehaviorName != MappableBehavior->BehaviorName) continue;
			if (SavedOperationArgs.Slot != InSlot) continue;

			OutOperationArgs = SavedOperationArgs;
			OutIMCTag = TaggedOperationArgs.IMCTag;
			return true;
		}
	}

	return false;
}

bool UInputManager::SaveMapPlayerOperationArgs(const FGameplayTag& IMCTag, const FMapPlayerOperationArgs& MapPlayerOperationArgs)
{
	if (!LoadedMappableKeySetting)
	{
		LoadedMappableKeySetting = Cast<UProjectMappableKeySetting>(UGameplayStatics::CreateSaveGameObject(UProjectMappableKeySetting::StaticClass()));
		if (!LoadedMappableKeySetting) return false;
	}

	FTaggedMapPlayerOperationArgs* TaggedOperationArgs = FindOrAddTaggedOperationArgs(IMCTag);
	if (!TaggedOperationArgs) return false;

	for (FMapPlayerOperationArgs& SavedOperationArgs : TaggedOperationArgs->OperationArgsArray)
	{
		if (SavedOperationArgs.BehaviorName != MapPlayerOperationArgs.BehaviorName) continue;
		if (SavedOperationArgs.Slot != MapPlayerOperationArgs.Slot) continue;

		SavedOperationArgs = MapPlayerOperationArgs;
		const FString SaveSlotName = UProjectGameInstance::GetSaveSlotName(EProjectSaveSlotName::MappableKey);
		return UGameplayStatics::SaveGameToSlot(LoadedMappableKeySetting, SaveSlotName, 0);
	}

	TaggedOperationArgs->OperationArgsArray.Add(MapPlayerOperationArgs);

	const FString SaveSlotName = UProjectGameInstance::GetSaveSlotName(EProjectSaveSlotName::MappableKey);
	return UGameplayStatics::SaveGameToSlot(LoadedMappableKeySetting, SaveSlotName, 0);
}

bool UInputManager::ApplyMappedInputContextOperation(
	UInputMappingContext* InputMappingContext,
	const FGameplayTag& IMCTag,
	const FMapPlayerOperationArgs& MapPlayerOperationArgs)
{
	if (!InputMappingContext) return false;

	const FName RuntimeMappingName = BuildRuntimeMappingNameBySlot(MapPlayerOperationArgs.BehaviorName, MapPlayerOperationArgs.Slot);
	if (RuntimeMappingName.IsNone()) return false;

	TMap<FName, int32>& RuntimeIndexMap = RuntimeMappedKeyIndexMap.FindOrAdd(IMCTag);

	int32 FoundMappingIndex = INDEX_NONE;
	if (const int32* FoundMappingIndexPtr = RuntimeIndexMap.Find(RuntimeMappingName))
		FoundMappingIndex = *FoundMappingIndexPtr;

	if (FoundMappingIndex == INDEX_NONE)
	{
		const UMappingBehavior* MappableBehavior = FindBehaviorByNameAndIMCTag(MapPlayerOperationArgs.BehaviorName, IMCTag);
		if (!MappableBehavior || !MappableBehavior->InputAction) return false;

		// 运行时 IMC 中不存在这条映射时，先新增一条 Mapping
		InputMappingContext->MapKey(MappableBehavior->InputAction, MapPlayerOperationArgs.NewKey);
		FoundMappingIndex = InputMappingContext->GetMappings().Num() - 1;
		RuntimeIndexMap.Add(RuntimeMappingName, FoundMappingIndex);
	}

	if (!InputMappingContext->GetMappings().IsValidIndex(FoundMappingIndex)) return false;

	const UMappingBehavior* MappableBehavior = FindBehaviorByNameAndIMCTag(MapPlayerOperationArgs.BehaviorName, IMCTag);
	if (!MappableBehavior) return false;

	FEnhancedActionKeyMapping& InputMapping = InputMappingContext->GetMapping(FoundMappingIndex);
	CopyBehaviorTemplateToMapping(MappableBehavior, InputMappingContext, InputMapping);
	InputMapping.Key = MapPlayerOperationArgs.NewKey;

	if (MapPlayerOperationArgs.bAlt)
	{
		UInputTriggerChordAction* AltChordInputTrigger = NewObject<UInputTriggerChordAction>(InputMappingContext);
		AltChordInputTrigger->ChordAction = CachedInputInfo->AltTriggerAction;
		InputMapping.Triggers.Add(AltChordInputTrigger);
	}

	if (MapPlayerOperationArgs.bControl)
	{
		UInputTriggerChordAction* ControlChordInputTrigger = NewObject<UInputTriggerChordAction>(InputMappingContext);
		ControlChordInputTrigger->ChordAction = CachedInputInfo->ControlTriggerAction;
		InputMapping.Triggers.Add(ControlChordInputTrigger);
	}

	if (MapPlayerOperationArgs.bShift)
	{
		UInputTriggerChordAction* ShiftChordInputTrigger = NewObject<UInputTriggerChordAction>(InputMappingContext);
		ShiftChordInputTrigger->ChordAction = CachedInputInfo->ShiftTriggerAction;
		InputMapping.Triggers.Add(ShiftChordInputTrigger);
	}

	return true;
}

int32 UInputManager::FindRuntimeMappingIndex(const FGameplayTag& IMCTag, const FName RuntimeMappingName) const
{
	if (!IMCTag.IsValid() || RuntimeMappingName.IsNone()) return INDEX_NONE;

	const TMap<FName, int32>* RuntimeIndexMap = RuntimeMappedKeyIndexMap.Find(IMCTag);
	if (!RuntimeIndexMap) return INDEX_NONE;

	const int32* FoundMappingIndex = RuntimeIndexMap->Find(RuntimeMappingName);
	if (!FoundMappingIndex) return INDEX_NONE;

	return *FoundMappingIndex;
}

FName UInputManager::BuildRuntimeMappingName(const FName BaseMappingName, const int32 OccurrenceIndex)
{
	if (OccurrenceIndex == 0)
		return FName(*FString::Printf(TEXT("%s_First"), *BaseMappingName.ToString()));
	if (OccurrenceIndex == 1)
		return FName(*FString::Printf(TEXT("%s_Second"), *BaseMappingName.ToString()));
	return NAME_None;
}

FName UInputManager::BuildRuntimeMappingNameBySlot(const FName BaseMappingName, const EPlayerMappableKeySlot InSlot)
{
	return BuildRuntimeMappingName(BaseMappingName, static_cast<int32>(InSlot));
}

void UInputManager::HandleChordFlagsFromMapping(const FEnhancedActionKeyMapping& InputMapping, FInputChord& OutInputChord) const
{
	OutInputChord.bAlt = false;
	OutInputChord.bCtrl = false;
	OutInputChord.bShift = false;

	if (!CachedInputInfo) return;

	for (const UInputTrigger* InputTrigger : InputMapping.Triggers)
	{
		const UInputTriggerChordAction* ChordTrigger = Cast<UInputTriggerChordAction>(InputTrigger);
		if (!ChordTrigger) continue;

		if (ChordTrigger->ChordAction == CachedInputInfo->AltTriggerAction)
			OutInputChord.bAlt = true;
		else if (ChordTrigger->ChordAction == CachedInputInfo->ControlTriggerAction)
			OutInputChord.bCtrl = true;
		else if (ChordTrigger->ChordAction == CachedInputInfo->ShiftTriggerAction)
			OutInputChord.bShift = true;
	}
}

void UInputManager::CopyBehaviorTemplateToMapping(
	const UMappingBehavior* MappableBehavior,
	UInputMappingContext* InputMappingContext,
	FEnhancedActionKeyMapping& InputMapping) const
{
	if (!MappableBehavior || !InputMappingContext) return;

	InputMapping.Modifiers.Empty();
	InputMapping.Triggers.Empty();

	for (UInputModifier* InputModifier : MappableBehavior->InputModifiers)
	{
		if (!InputModifier) continue;
		InputMapping.Modifiers.Add(DuplicateObject(InputModifier, InputMappingContext));
	}

	for (UInputTrigger* InputTrigger : MappableBehavior->InputTriggers)
	{
		if (!InputTrigger || IsPlayerChordTrigger(InputTrigger)) continue;
		InputMapping.Triggers.Add(DuplicateObject(InputTrigger, InputMappingContext));
	}
}

bool UInputManager::IsPlayerChordTrigger(const UInputTrigger* InputTrigger) const
{
	const UInputTriggerChordAction* ChordTrigger = Cast<UInputTriggerChordAction>(InputTrigger);
	if (!ChordTrigger || !CachedInputInfo) return false;

	return ChordTrigger->ChordAction == CachedInputInfo->AltTriggerAction ||
		ChordTrigger->ChordAction == CachedInputInfo->ControlTriggerAction ||
		ChordTrigger->ChordAction == CachedInputInfo->ShiftTriggerAction;
}

bool UInputManager::IsOperationArgsMatchingChord(const FMapPlayerOperationArgs& OperationArgs, const FInputChord& InputChord) const
{
	return OperationArgs.NewKey == InputChord.Key &&
		OperationArgs.bAlt == InputChord.bAlt &&
		OperationArgs.bControl == InputChord.bCtrl &&
		OperationArgs.bShift == InputChord.bShift;
}

void UInputManager::ClearOperationArgs(FMapPlayerOperationArgs& OperationArgs) const
{
	OperationArgs.NewKey = EKeys::Invalid;
	OperationArgs.bAlt = false;
	OperationArgs.bControl = false;
	OperationArgs.bShift = false;
}

bool UInputManager::ClearDuplicateKeyByBehaviorPure(
	const UMappingBehavior* MappableBehavior,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord,
	bool& bOutHasChanged)
{
	bOutHasChanged = false;
	if (!LoadedMappableKeySetting || !MappableBehavior || MappableBehavior->BehaviorName.IsNone()) return false;

	FTaggedMapPlayerOperationArgs* TaggedOperationArgs = FindOrAddTaggedOperationArgs(MappableBehavior->IMCTag);
	if (!TaggedOperationArgs) return false;

	// 只清理同一行为里“另一槽位且键位完全相同”的记录
	for (FMapPlayerOperationArgs& SavedOperationArgs : TaggedOperationArgs->OperationArgsArray)
	{
		if (SavedOperationArgs.BehaviorName != MappableBehavior->BehaviorName) continue;
		if (SavedOperationArgs.Slot == InSlot) continue;
		if (!IsOperationArgsMatchingChord(SavedOperationArgs, SelectedInputChord)) continue;

		ClearOperationArgs(SavedOperationArgs);
		bOutHasChanged = true;
	}

	return true;
}

bool UInputManager::ClearDuplicateKeyByBehavior(
	const UMappingBehavior* MappableBehavior,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord)
{
	bool bHasChanged = false;
	if (!ClearDuplicateKeyByBehaviorPure(MappableBehavior, InSlot, SelectedInputChord, bHasChanged)) return false;
	if (!bHasChanged) return true;

	const FString SaveSlotName = UProjectGameInstance::GetSaveSlotName(EProjectSaveSlotName::MappableKey);
	if (!UGameplayStatics::SaveGameToSlot(LoadedMappableKeySetting, SaveSlotName, 0)) return false;
	if (!RebuildRuntimeInputContextByTag(MappableBehavior->IMCTag)) return false;

	return true;
}

void UInputManager::RefreshControlMappings() const
{
	if (!CachedEnhancedInputLocalPlayerSubsystem.IsValid()) return;

	CachedEnhancedInputLocalPlayerSubsystem->RequestRebuildControlMappings(
		FModifyContextOptions(),
		EInputMappingRebuildType::RebuildWithFlush);
}
