#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"

class UInputMappingContext;
struct FGameplayTag;
class UGameplayEffect;
class UGameplayAbility;

// ==========================================
// Player Category（玩家角色分类）
// ==========================================
UENUM(BlueprintType)
enum class EPlayerCategory
{
	Empty,
	Crouch,
	Wraith,
	Phase
};

// ==========================================
// Enemy Category（怪物分类）
// ==========================================
UENUM()
enum class EEnemyCategory
{
	Empty,	// 默认空
	Test	// 测试用怪物
};


// ==========================================
// Player 特有的初始化信息
// ==========================================
USTRUCT(BlueprintType)
struct FSpecialPlayerInfo
{
	GENERATED_BODY()
	/** 专门用于初始化 Level 属性的 GE */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> LevelAttributeEffectClass;

	/** 使用 MMC 计算属性值的 GE（如 MaxHealth/MaxMana 等，通过曲线查表） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> MMCAttributeEffectClass;

	/** 使用 Override 直接赋值的 GE（除 Level 外的其他固定值属性，如 Health/Mana 初始百分比等） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> OtherAttributeEffectClass;

	/** 该 Category 特有的主动技能（Offensive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> SpecialStartUpAbilities_Offensive;

	/** 该 Category 特有的被动技能（Passive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> SpecialStartUpAbilities_Passive;
};

// ==========================================
// Enemy 特有的初始化信息
// ==========================================
USTRUCT(BlueprintType)
struct FSpecialEnemyInfo
{
	GENERATED_BODY()
	/** 专门用于初始化 Level 属性的 GE（通过 SetByCaller 传入怪物等级） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> LevelAttributeEffectClass;

	/** 使用 MMC/AttributeBased 根据等级计算属性值的 GE */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> LevelBasedAttributeEffectClass;

	/** 使用 Override 直接赋值的 GE（除 Level 外的其他固定值属性） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> OtherAttributeEffectClass;

	/** 该 Category 特有的主动技能（Offensive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> SpecialStartUpAbilities_Offensive;

	/** 该 Category 特有的被动技能（Passive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> SpecialStartUpAbilities_Passive;
};


//在这个CharacterInfo里面，添加的GA都是非输入性的东西，输入性的东西都在InputInfo里面
UCLASS()
class P20260324_API UCharacterInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	// ==========================================
	// Player 相关数据
	// ==========================================

	/** Player Category → 对应特有信息的映射表 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EPlayerCategory, FSpecialPlayerInfo> PlayerCategoryToPlayerInfo;

	/** 所有 Player Category 共享的通用非输入性主动技能（Offensive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> CommonPlayerStartupAbilities_Offensive;

	/** 所有 Player Category 共享的通用非输入性被动技能（Passive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> CommonPlayerStartupAbilities_Passive;
	
	UFUNCTION(BlueprintCallable, Category = "Character Info")
	bool FindSpecialPlayerInfoByCategory(EPlayerCategory PlayerCategory, FSpecialPlayerInfo& OutPlayerInfo) const;

	/** 获取 Player 通用主动技能 */
	UFUNCTION(BlueprintCallable, Category = "Character|Player|Ability")
	const TArray<TSubclassOf<UGameplayAbility>>& GetCommonPlayerStartupAbilities_Offensive() const { return CommonPlayerStartupAbilities_Offensive; }

	/** 获取 Player 通用被动技能 */
	UFUNCTION(BlueprintCallable, Category = "Character|Player|Ability")
	const TArray<TSubclassOf<UGameplayAbility>>& GetCommonPlayerStartupAbilities_Passive() const { return CommonPlayerStartupAbilities_Passive; }


	// ==========================================
	// Enemy 相关数据
	// ==========================================

	/** Enemy Category → 对应特有信息的映射表 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EEnemyCategory, FSpecialEnemyInfo> EnemyCategoryToEnemyInfo;

	/** 所有 Enemy Category 共享的通用主动技能（Offensive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> CommonEnemyStartupAbilities_Offensive;

	/** 所有 Enemy Category 共享的通用被动技能（Passive） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> CommonEnemyStartupAbilities_Passive;
	
	UFUNCTION(BlueprintCallable, Category = "Character Info")
	bool FindSpecialEnemyInfoByCategory(EEnemyCategory EnemyCategory, FSpecialEnemyInfo& OutEnemyInfo) const;
	
	/** 获取 Enemy 通用主动技能 */
	UFUNCTION(BlueprintCallable, Category = "Character|Enemy|Ability")
	const TArray<TSubclassOf<UGameplayAbility>>& GetCommonEnemyStartupAbilities_Offensive() const { return CommonEnemyStartupAbilities_Offensive; }

	/** 获取 Enemy 通用被动技能 */
	UFUNCTION(BlueprintCallable, Category = "Character|Enemy|Ability")
	const TArray<TSubclassOf<UGameplayAbility>>& GetCommonEnemyStartupAbilities_Passive() const { return CommonEnemyStartupAbilities_Passive; }

};
