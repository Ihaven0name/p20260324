


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Info/InputInfo.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "Info/CharacterInfo.h"
#include "Interface/CharacterInterface.h"
#include "ProjectBaseCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnProjectCharacterDied, AActor*);

class UAbilitySystemComponent;
class UProjectAbilitySystemComponent;
class UAnimMontage;

UCLASS()
class P20260324_API AProjectBaseCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AProjectBaseCharacter();

	/** 角色当前是否存在非零移动速度，由服务器根据 CharacterMovementComponent 更新并复制。 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="动画|移动状态")
	bool HasVelocity = false;

	/** 角色当前是否存在非零移动加速度，由服务器根据 CharacterMovementComponent 更新并复制。 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="动画|移动状态")
	bool HasAcceleration = false;

	/** 角色是否已死亡，由服务器设置并复制给客户端。 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IsDead, Category="状态")
	bool bIsDead = false;

	/** 仅在服务器确认死亡时广播；供 AI 等服务器逻辑监听。 */
	FOnProjectCharacterDied OnCharacterDied;

	/** 在服务器上通过接口将角色标记为死亡；重复调用会被忽略。 */
	virtual void SetIsDead_Implementation() override;
	virtual bool IsDead_Implementation() const override { return bIsDead; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnDeathStateChanged();
	UFUNCTION()
	void OnRep_IsDead();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** 在 CharacterMovementComponent 完成本次移动更新后刷新并复制移动状态。 */
	UFUNCTION()
	void UpdateMovementStateFromCMC(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);

public:
	// ICharacterInterface
	virtual int32 GetLevel_Implementation() override;
	virtual void AddLevel_Implementation(const int32 InLevel) override;
	virtual int32 GetXP_Implementation() override;
	virtual void AddXP_Implementation(const int32 InXP) override;
	virtual UProjectAbilitySystemComponent* GetAbilitySystemComponentByInterface_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation(EHitReactDirection Direction) override;
	virtual UAnimMontage* GetDeathMontage_Implementation() override;
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

	UPROPERTY(BlueprintReadWrite, Category="Attribute")
	TObjectPtr<UProjectAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Hit React")
	TObjectPtr<UAnimMontage> FrontHitReactMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Hit React")
	TObjectPtr<UAnimMontage> BackHitReactMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Hit React")
	TObjectPtr<UAnimMontage> LeftHitReactMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Hit React")
	TObjectPtr<UAnimMontage> RightHitReactMontage;

	/** 死亡表现 Montage 的接口配置，供死亡能力或流程调用。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|Death")
	TObjectPtr<UAnimMontage> DeathMontage;



};
