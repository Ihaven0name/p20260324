


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Info/InputInfo.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "Info/CharacterInfo.h"
#include "Interface/CharacterInterface.h"
#include "ProjectBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UProjectAbilitySystemComponent;

UCLASS()
class P20260324_API AProjectBaseCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AProjectBaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// ICharacterInterface
	virtual int32 GetLevel_Implementation() override;
	virtual void AddLevel_Implementation(const int32 InLevel) override;
	virtual int32 GetXP_Implementation() override;
	virtual void AddXP_Implementation(const int32 InXP) override;
	virtual UProjectAbilitySystemComponent* GetAbilitySystemComponentByInterface_Implementation() override;
	virtual void AddInputWindowTimer_Implementation(const float InRate, const FGameplayTagContainer& InTags) override;
	/**
	 * 初始化 Ability Actor Info，子类实现
	 */
	virtual void InitAbilityActorInfo() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName WeaponSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName RightHandSocketName;
	UPROPERTY(BlueprintReadWrite)
	bool bIsCrouching=false;


	UPROPERTY()
	FTimerHandle InputWindowTimerHandle;
	UPROPERTY()
	FGameplayTagContainer InputWindowTagContainer;


	UFUNCTION(BlueprintCallable)
	UProjectAbilitySystemComponent* GetAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	UProjectAttributeSet* GetAttributeSet();
	
	UFUNCTION()
	void OnRep_Level(const int32 OldLevel);
	UFUNCTION()
	void OnRep_XP(const int32 OldXP);
	UFUNCTION()
	void RemoveInputWindowTags();

	// ========== 等级 & 经验 ==========
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Level, Category="Level")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_XP, Category="XP")
	int32 XP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	TObjectPtr<UProjectAttributeSet> AttributeSet;



};
