#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/Commands/InputChord.h"
#include "Info/MappableInfo.h"
#include "Input/ProjectMappableKeySetting.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputManager.generated.h"

class APlayerController;
class UEnhancedInputLocalPlayerSubsystem;
class UInputInfo;
class UInputMappingContext;
class UInputTrigger;
class UMappingBehavior;
class UProjectMappableKeySetting;
struct FEnhancedActionKeyMapping;

USTRUCT(BlueprintType)
struct FGroupTagMappedKeys
{
	GENERATED_BODY()

	// 当前这组可映射行为对应的组标签
	UPROPERTY(BlueprintReadOnly, Category="Input|Mappable")
	FGameplayTag GroupTag;

	// 主槽位当前显示的按键
	UPROPERTY(BlueprintReadOnly, Category="Input|Mappable")
	FInputChord PrimaryInputChord;

	// 次槽位当前显示的按键
	UPROPERTY(BlueprintReadOnly, Category="Input|Mappable")
	FInputChord SecondaryInputChord;

	// 是否存在有效的主槽位按键
	UPROPERTY(BlueprintReadOnly, Category="Input|Mappable")
	bool bHasPrimaryKey = false;

	// 是否存在有效的次槽位按键
	UPROPERTY(BlueprintReadOnly, Category="Input|Mappable")
	bool bHasSecondaryKey = false;
};

UCLASS()
class P20260324_API UInputManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 运行时实际使用的 IMC，键位修改后会重建到这里
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputMappingContext")
	TMap<FGameplayTag, TObjectPtr<UInputMappingContext>> IMCTagToMappedInputMappingContext;

	UFUNCTION(BlueprintPure, Category="Input|Mappable")
	UMappableInfo* GetMappableInfo() const { return CachedMappableInfo; }

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool InitializeMappableByPlayerController(APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool GetCurrentKeysByGroupTag(FGameplayTag GroupTag, FInputChord& OutPrimaryInputChord, FInputChord& OutSecondaryInputChord) const;

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool GetMappedKeysDataByGroupTag(FGameplayTag GroupTag, FGroupTagMappedKeys& OutMappedKeys) const;

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool GetCurrentPrimaryKeyByGroupTag(FGameplayTag GroupTag, FInputChord& OutInputChord) const;

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool GetCurrentSecondaryKeyByGroupTag(FGameplayTag GroupTag, FInputChord& OutInputChord) const;

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool GetCurrentKeyByGroupTagWithSlot(FGameplayTag GroupTag, EPlayerMappableKeySlot InSlot, FInputChord& OutInputChord) const;

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool RemapKeyByGroupTag(FGameplayTag GroupTag, const FInputChord& SelectedInputChord);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool RemapPrimaryKeyByGroupTag(FGameplayTag GroupTag, const FInputChord& SelectedInputChord);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool RemapSecondaryKeyByGroupTag(FGameplayTag GroupTag, const FInputChord& SelectedInputChord);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool RemapKeyByGroupTagOneSlot(FGameplayTag GroupTag, const FInputChord& SelectedInputChord);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool RemapKeyByGroupTagWithSlot(FGameplayTag GroupTag, EPlayerMappableKeySlot InSlot, const FInputChord& SelectedInputChord);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool RemapKeyByGroupTagWithSlotAndOutputClearedSlot(
		FGameplayTag GroupTag,
		EPlayerMappableKeySlot InSlot,
		const FInputChord& SelectedInputChord,
		bool& bOutHasClearedDuplicateSlot,
		EPlayerMappableKeySlot& OutClearedSlot);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool ClearKeyByGroupTagWithSlot(FGameplayTag GroupTag, EPlayerMappableKeySlot InSlot);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool ActivateMappedInputMappingContextByTag(FGameplayTag IMCTag, int32 Priority = 0);

	UFUNCTION(BlueprintCallable, Category="Input|Mappable")
	bool DeactivateMappedInputMappingContextByTag(FGameplayTag IMCTag);

protected:
	// 可映射行为的数据配置
	UPROPERTY(Transient)
	TObjectPtr<UMappableInfo> CachedMappableInfo;

	// 当前已加载的改键存档
	UPROPERTY(Transient)
	TObjectPtr<UProjectMappableKeySetting> LoadedMappableKeySetting;

	// 输入配置总表
	UPROPERTY(Transient)
	TObjectPtr<UInputInfo> CachedInputInfo;

	// 当前本地玩家的 Enhanced Input 子系统
	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> CachedEnhancedInputLocalPlayerSubsystem;

	// 运行时 IMC 中的 MappingName 到 MappingIndex 的快速索引
	TMap<FGameplayTag, TMap<FName, int32>> RuntimeMappedKeyIndexMap;

protected:
	// 根据 GroupTag 找到可映射组配置
	const FMappableActionGroup* GetMappableGroupByTag(FGameplayTag GroupTag) const;

	// 根据 BehaviorName 和 IMCTag 找到具体行为配置
	const UMappingBehavior* FindBehaviorByNameAndIMCTag(FName BehaviorName, FGameplayTag IMCTag) const;

	// 读取某个行为某个槽位当前使用的按键信息
	bool GetCurrentKeyByBehavior(const UMappingBehavior* MappableBehavior, EPlayerMappableKeySlot InSlot, FInputChord& OutInputChord) const;

	// 单槽位或不需要去重的改键入口
	bool RemapKeyByBehavior(const UMappingBehavior* MappableBehavior, EPlayerMappableKeySlot InSlot, const FInputChord& SelectedInputChord);

	// 双槽位改键入口，会额外返回是否清理了重复槽位
	bool RemapKeyByBehaviorAndOutputClearedSlot(
		const UMappingBehavior* MappableBehavior,
		EPlayerMappableKeySlot InSlot,
		const FInputChord& SelectedInputChord,
		bool& bOutHasClearedDuplicateSlot,
		EPlayerMappableKeySlot& OutClearedSlot);

	// 实际执行改键的内部函数，通过 bHandleDuplicate 控制是否处理重复槽位
	bool RemapKeyByBehaviorInternal(
		const UMappingBehavior* MappableBehavior,
		EPlayerMappableKeySlot InSlot,
		const FInputChord& SelectedInputChord,
		bool bHandleDuplicate,
		bool& bOutHasClearedDuplicateSlot,
		EPlayerMappableKeySlot& OutClearedSlot);

	// 清空某个行为某个槽位的按键
	bool ClearKeyByBehavior(const UMappingBehavior* MappableBehavior, EPlayerMappableKeySlot InSlot);

	// 仅在内存中的存档数据里清理重复键，不保存也不重建
	bool ClearDuplicateKeyByBehaviorPure(
		const UMappingBehavior* MappableBehavior,
		EPlayerMappableKeySlot InSlot,
		const FInputChord& SelectedInputChord,
		bool& bOutHasChanged);

	// 清理重复键，并同步保存与重建运行时 IMC
	bool ClearDuplicateKeyByBehavior(
		const UMappingBehavior* MappableBehavior,
		EPlayerMappableKeySlot InSlot,
		const FInputChord& SelectedInputChord);

	// 首次创建改键存档时，把默认 IMC 中的可映射键位写入存档
	bool CreateMappableKeySlot();

	// 根据当前存档重建所有运行时 IMC
	bool RebuildRuntimeInputContextsFromSaveData();

	// 只重建某一个 IMCTag 对应的运行时 IMC
	bool RebuildRuntimeInputContextByTag(const FGameplayTag& IMCTag);

	// 查找指定 IMCTag 的存档数据，不存在就创建
	FTaggedMapPlayerOperationArgs* FindOrAddTaggedOperationArgs(const FGameplayTag& IMCTag);

	// 只查找指定 IMCTag 的存档数据
	const FTaggedMapPlayerOperationArgs* FindTaggedOperationArgs(const FGameplayTag& IMCTag) const;

	// 根据行为和槽位，从存档中取出当前操作参数
	bool HandleOperationArgsByBehavior(
		const UMappingBehavior* MappableBehavior,
		EPlayerMappableKeySlot InSlot,
		FMapPlayerOperationArgs& OutOperationArgs,
		FGameplayTag& OutIMCTag) const;

	// 保存一条改键记录到存档
	bool SaveMapPlayerOperationArgs(const FGameplayTag& IMCTag, const FMapPlayerOperationArgs& MapPlayerOperationArgs);

	// 把一条改键记录应用到运行时 IMC
	bool ApplyMappedInputContextOperation(
		UInputMappingContext* InputMappingContext,
		const FGameplayTag& IMCTag,
		const FMapPlayerOperationArgs& MapPlayerOperationArgs);

	// 在运行时索引表里查找某个 MappingName 对应的下标
	int32 FindRuntimeMappingIndex(const FGameplayTag& IMCTag, const FName RuntimeMappingName) const;

	// 根据同一行为出现次数构造运行时 MappingName
	static FName BuildRuntimeMappingName(const FName BaseMappingName, int32 OccurrenceIndex);

	// 根据槽位构造运行时 MappingName
	static FName BuildRuntimeMappingNameBySlot(const FName BaseMappingName, EPlayerMappableKeySlot InSlot);

	// 从 Mapping 的 Trigger 中提取 Ctrl / Alt / Shift 信息
	void HandleChordFlagsFromMapping(const FEnhancedActionKeyMapping& InputMapping, FInputChord& OutInputChord) const;

	// 把行为模板里的 Trigger / Modifier 拷贝到运行时 Mapping
	void CopyBehaviorTemplateToMapping(
		const UMappingBehavior* MappableBehavior,
		UInputMappingContext* InputMappingContext,
		FEnhancedActionKeyMapping& InputMapping) const;

	// 判断一个 Trigger 是否是玩家改键系统使用的弦操作 Trigger
	bool IsPlayerChordTrigger(const UInputTrigger* InputTrigger) const;

	// 判断一条存档记录和一个输入弦是否完全一致
	bool IsOperationArgsMatchingChord(const FMapPlayerOperationArgs& OperationArgs, const FInputChord& InputChord) const;

	// 把一条改键记录清空为无效输入
	void ClearOperationArgs(FMapPlayerOperationArgs& OperationArgs) const;

	// 通知 Enhanced Input 重新刷新控制映射
	void RefreshControlMappings() const;
};

//TODO:记录一下我的修改键位功能是怎么实现的
//TODO:我把runstart,runstop,runpivot的同步组关闭了,同步组似乎就是给循环动画用的,pivot的动画有问题
