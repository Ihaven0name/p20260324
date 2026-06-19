
#pragma once

#include "CoreMinimal.h"
#include "ProjectPlayerCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ProjectPlayerController.generated.h"


class AProjectAttackHUD;
class AProjectPlayerCharacter;
class UProjectGameInstance;
struct FInputActionValue;
class UProjectAbilitySystemComponent;
class UInputAction;

UCLASS()
class P20260324_API AProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AProjectPlayerController();
	

protected:
	virtual void BeginPlay() override;

	
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
public:
	UPROPERTY()
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UProjectGameInstance> GameInstance;
	UPROPERTY()
	TObjectPtr<AProjectPlayerCharacter> PlayerCharacter;
	UPROPERTY()
	TObjectPtr<AProjectAttackHUD> ProjectHUD;

	UPROPERTY(BlueprintReadWrite)
	bool bIsLocking=false;
	//TODO:需要在怪物四的时候移出LockingActor
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> LockingActor=nullptr;
	UPROPERTY(BlueprintReadWrite)
	float SwitchTargetNumber=0.f;
	
	// ==================== 预输入相关 ====================
	// 预输入是否可用（由 Ability 在合适时机设置，如攻击动画开始时）
	UPROPERTY(BlueprintReadWrite)
	bool bCanPreInput=false;
	// 最后一个按下的 AbilityTag（用于预输入）
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayTag> PreInputTags;
	UFUNCTION(BlueprintCallable)
	void AddAbilityTagToPreInput(const FGameplayTag AbilityTag);
	UFUNCTION(BlueprintCallable)
	bool FindActivatableAbilityTag();
	// ==================== 预输入相关 ====================

	UPROPERTY(EditDefaultsOnly, Category="LockOn")
	float MaxLockDistance = 1500.0f;  // 最大锁定距离
	UPROPERTY(EditDefaultsOnly, Category="LockOn")
	float MaxLockAngle = 60.0f;  // 扇形角度
	UPROPERTY(EditDefaultsOnly, Category="LockOn")
	float MouseDecaySpeed = 8.0f;  // 鼠标累计值衰减速度
	UPROPERTY(EditDefaultsOnly, Category="LockSwitch")
	float SwitchTargetThreshold=500.f;
	UPROPERTY(EditDefaultsOnly, Category="LockSwitch")
	float LockOnPitchRadius=-20.f;
	
	// 锁定时的插值相关变量
	UPROPERTY()
	FRotator LerpCurrentRotation;  // 当前插值的旋转
	UPROPERTY(EditDefaultsOnly, Category="LockOn")
	float RotationInterpSpeed = 10.0f;  // 旋转插值速度
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> SpringArmScaleAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> LockOnAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
    TObjectPtr<UInputAction> LockSwitchAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> ChangeAttributeUIAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> ChangeSettingUIAction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	float AttributeUICreateDuration=0.5f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	float SettingUICreateDuration=0.5f;
	UPROPERTY()
	FTimerHandle AttributeUITimerHandle;
	UPROPERTY()
	FTimerHandle SettingUITimerHandle;
	UPROPERTY()
	bool bCanGenerateAttributeUI=true;
	UPROPERTY()
	bool bCanGenerateSettingUI=true;

	// 当前激活的角色专属 IMC Tag（用于切换时 Remove）
	UPROPERTY(BlueprintReadOnly, Category="Input")
	FGameplayTag CurrentSpecialIMCTag;

	/** 切换角色输入：移除旧角色的 SpecialIMC，添加新角色的 */
	UFUNCTION(BlueprintCallable)
	void SwitchCharacterIMC(EPlayerCategory NewCategory);


	UFUNCTION(BlueprintCallable)
	UProjectAbilitySystemComponent* GetAbilitySystemComponent();
	UFUNCTION(BlueprintCallable)
	UProjectGameInstance* GetProjectGameInstance();
	UFUNCTION(BlueprintCallable)
	AProjectPlayerCharacter* GetPlayerCharacter();
	UFUNCTION(BlueprintCallable)
	AProjectAttackHUD* GetProjectHUD();

	
	void MoveActionFunction(const FInputActionValue& Value);
	void LookActionFunction(const FInputActionValue& Value);
	void SpringArmScaleActionFunction(const FInputActionValue& Value);
	void JumpActionFunction(const FInputActionValue& Value);
	void LockOnActionFunction(const FInputActionValue& Value);
	void LockSwitchActionFunction(const FInputActionValue& Value);

	// 技能输入回调（接收 AbilityTag）— 原有逻辑，绑定目标 Ability
	void OnAbilityInputStarted(const FGameplayTag AbilityTag);
	void OnAbilityInputReleased(const FGameplayTag AbilityTag);
	void OnAbilityInputTriggered(const FGameplayTag AbilityTag);

	// Judge 输入回调 — 绑定 Judge Ability（用于预输入判断：短按/长按）
	/**
	 * 按下 InputAction → 激活对应的 Judge Ability
	 * @param JudgeTag Judge Ability 自身的 AbilityTag（与 FInputActionAndAbilityInputData 中 JudgeAbility 的标签一致）
	 */
	void OnAbilityJudgeStarted(const FGameplayTag JudgeTag);

	/**
	 * 释放 InputAction → 通知 ASC 释放事件（Judge 内部 AbilityTask_WaitReleaseAtDuration 监听此事件）
	 * @param JudgeTag Judge Ability 的 AbilityTag
	 */
	void OnAbilityJudgeReleased(const FGameplayTag JudgeTag);

	/**
	 * 更新锁定时的旋转插值
	 * @param DeltaTime 时间增量
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateLockOnRotationInterpolation(float DeltaTime);


	UFUNCTION(Server, Reliable)
	void Server_SwitchCharacter(int32 TargetIndex);
	
};


