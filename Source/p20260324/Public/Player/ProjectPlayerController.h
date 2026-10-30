
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
class UTargetLockComponent;
class UEnemyVisibilityComponent;

UCLASS()
class P20260324_API AProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AProjectPlayerController();
	

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

	
	virtual void SetupInputComponent() override;
public:
	UPROPERTY()
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UProjectGameInstance> GameInstance;
	UPROPERTY()
	TObjectPtr<AProjectPlayerCharacter> PlayerCharacter;
	UPROPERTY()
	TObjectPtr<AProjectAttackHUD> ProjectHUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="组件", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTargetLockComponent> TargetLockComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="组件", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEnemyVisibilityComponent> EnemyVisibilityComponent;

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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> WheelAction;
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
	UFUNCTION(BlueprintPure, Category="组件")
	UTargetLockComponent* GetTargetLockComponent() const { return TargetLockComponent; }
	UFUNCTION(BlueprintPure, Category="组件")
	UEnemyVisibilityComponent* GetEnemyVisibilityComponent() const { return EnemyVisibilityComponent; }

	
	void MoveActionFunction(const FInputActionValue& Value);
	void LookActionFunction(const FInputActionValue& Value);
	void WheelActionFunction(const FInputActionValue& Value);
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

	UFUNCTION(Server, Reliable)
	void Server_SwitchCharacter(int32 TargetIndex);

	/** 本地死亡界面的重开请求；实际重载由服务器执行。 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RequestRestartGame();

	UFUNCTION(Server, Reliable)
	void Server_RequestRestartGame();
	void RefreshMainAttackUIWidgetController();

private:
	bool IsGameplayInputBlocked() const;
};


