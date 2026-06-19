#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Player/ProjectPlayerState.h"
#include "ProjectAbilitySystemComponent.h"
#include "ProjectGameplayTag.h"
#include "UITag.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/InventoryUIWidgetController.h"
#include "ProjectBlueprintFunctionLibrary.generated.h"

class UTeamManager;
class AProjectPlayerCharacter;
class UProjectAttributeSet;
class UProjectBaseWidgetController;
class UAttributeWidgetController;
class UProjectUserWidget;
struct FDamageEffectParam;
class UAudioManager;
class UConfigManager;
class UInputManager;
class UInventoryManager;
class ULevelManager;
class UObjectPoolManager;
class USaveManager;
class UUIManager;
class UEnemyManager;
class AProjectAttackHUD;
class UMainAttackUIWidgetController;
class USettingUIWidgetController;
struct FWidgetControllerParam;

/**
 * Project Blueprint Function Library
 */
UCLASS()
class P20260324_API UProjectBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static float GetDistanceToGround(const AActor* Actor);

	// Manager Getters
	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UAudioManager* GetAudioManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UConfigManager* GetConfigManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UInputManager* GetInputManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UInventoryManager* GetInventoryManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static ULevelManager* GetLevelManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UObjectPoolManager* GetObjectPoolManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static USaveManager* GetSaveManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UUIManager* GetUIManager(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UEnemyManager* GetEnemyManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static UTeamManager* GetTeamManager(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Manager")
	static void CalculateCurrentLevel(UProjectAttributeSet* AttributeSet,int32 TempXP, int& OutLevel, int& OutXP);


	/**
	 * 在数组中查找最近的 Actor
	 * @param OriginalActor 起始点 Actor
	 * @param TargetActors 目标 Actor 数组
	 * @return 最近的 Actor，如果没有则返回 nullptr
	 */
	UFUNCTION(BlueprintCallable)
	static AActor* FindNearestActor(const AActor* OriginalActor, const TArray<AActor*>& TargetActors);

	/**
	 * 根据AbilityTag检查对应的Ability是否能激活
	 * @param ASC AbilitySystemComponent指针
	 * @param AbilityTag 要检查的AbilityTag
	 * @param OutSpec 找到的AbilitySpec（仅在返回true时有效）
	 * @return CanActivateAbility的结果
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static bool CanActivateAbilityAndOutputSpec(UAbilitySystemComponent* ASC, const FGameplayTag& AbilityTag, FGameplayAbilitySpec& OutSpec);

	// ==========================================
	// Character Init: Abilities & Attributes
	// ==========================================

	/**
	 * 初始化 Player 的 Startup Abilities（通用 + Category 特有）
	 * @param ASC 目标 AbilitySystemComponent
	 * @param Category 角色的 EPlayerCategory
	 */
	UFUNCTION(BlueprintCallable, Category = "Project|GAS|Init")
	static void InitializePlayerStartupAbilities(UAbilitySystemComponent* ASC, EPlayerCategory Category);

	/**
	 * 初始化 Player 属性（三步：Level → MMC → Other Override）
	 * @param ASC 目标 AbilitySystemComponent
	 * @param Category 角色的 EPlayerCategory
	 */
	UFUNCTION(BlueprintCallable, Category = "Project|GAS|Init")
	static void InitializePlayerAttributeByEffect(UAbilitySystemComponent* ASC, EPlayerCategory Category);

	/**
	 * 初始化 Enemy 的 Startup Abilities（通用 + Category 特有）
	 * @param ASC 目标 AbilitySystemComponent
	 * @param Category 怪物的 EEnemyCategory
	 */
	UFUNCTION(BlueprintCallable, Category = "Project|GAS|Init")
	static void InitializeEnemyStartupAbilities(UAbilitySystemComponent* ASC, EEnemyCategory Category);

	/**
	 * 初始化 Enemy 属性（三步：Level → MMC → Other Override）
	 * @param ASC 目标 AbilitySystemComponent
	 * @param Category 怪物的 EEnemyCategory
	 * @param EnemyLevel 怪物实际等级，通过 SetByCaller 传入 GE 用于设置 Level 属性
	 */
	UFUNCTION(BlueprintCallable, Category = "Project|GAS|Init")
	static void InitializeEnemyAttributeByEffect(UAbilitySystemComponent* ASC, EEnemyCategory Category, int32 EnemyLevel = 1);

	// ==========================================
	// Damage Effect Context Set Functions
	// ==========================================
	UFUNCTION(BlueprintCallable, Category = "Project|GAS")
	static void SetKnockBackImpulse(FGameplayEffectContextHandle& ContextHandle, const FVector& InKnockBackImpulse);

	UFUNCTION(BlueprintPure, Category = "Project|GAS")
	static FVector GetKnockBackImpulse(const FGameplayEffectContextHandle& ContextHandle);

	// ==========================================
	// Damage Effect Apply Function
	// ==========================================
	UFUNCTION(BlueprintCallable, Category = "Project|GAS")
	static FGameplayEffectContextHandle ApplyDamageGameplayEffectByDamageEffectParam(const FDamageEffectParam& DamageEffectParam);

	// ==========================================
	// WidgetController Get Functions
	// ==========================================



	// --- MainAttack ---
	UFUNCTION(BlueprintCallable, Category = "UI|Controller")
	static UMainAttackUIWidgetController* GetMainAttackUIWidgetController(const AProjectPlayerController* InPlayerController);

	// --- Setting ---
	UFUNCTION(BlueprintCallable, Category = "UI|Controller")
	static USettingUIWidgetController* GetSettingUIWidgetController(const AProjectPlayerController* InPlayerController);

	// --- Attribute ---
	UFUNCTION(BlueprintCallable, Category = "UI|Controller")
	static UAttributeWidgetController* GetAttributeUIWidgetController(const AProjectPlayerController* InPlayerController);

	// --- Inventory ---
	UFUNCTION(BlueprintCallable, Category = "UI|Controller")
	static UInventoryUIWidgetController* GetInventoryUIWidgetController(const AProjectPlayerController* InPlayerController);

	// --- By Tag ---
	UFUNCTION(BlueprintCallable, Category = "UI|Controller")
	static UProjectBaseWidgetController* GetWidgetControllerByWidgetTag(const AProjectPlayerController* InPlayerController, const FGameplayTag& WidgetTag);

	UFUNCTION(BlueprintPure, Category = "Team")
	static TSoftObjectPtr<UTexture2D> GetTeamCharacterTexture(const FTeamCharacterInfo& TeamCharacterInfo);

	UFUNCTION(BlueprintCallable, Category = "Input")
    static void RebindOnCommonUIAction(
    	APlayerController* PC,
    	FUIActionTag ActionTag,
    	FName MappingName,
    	FKey NewKey
    );
	UFUNCTION(BlueprintCallable, Category = "Input")
	static void ApplyGameplayEffectSpecToSelfByClass(
		UProjectAbilitySystemComponent* AbilitySystemComponent,
		TSubclassOf<UGameplayEffect> GameplayEffectClass
	);
};
