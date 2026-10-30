



#pragma once

#include "CoreMinimal.h"
#include "ProjectBaseCharacter.h"
#include "Interface/PlayerInterface.h"
#include "ProjectPlayerCharacter.generated.h"


class AProjectPlayerController;
class AProjectPlayerState;
class USpringArmComponent;
class UCameraComponent;
class UPlayerInteractionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableTargetChanged, AActor*, PreviousTarget, AActor*, NewTarget);

UCLASS()
class P20260324_API AProjectPlayerCharacter : public AProjectBaseCharacter , public IPlayerInterface
{
	GENERATED_BODY()
public:
	
	AProjectPlayerCharacter();

	//Self
	virtual void InitAbilityActorInfo() override;
protected:
	virtual void OnDeathStateChanged() override;
	//Self
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void NotifyControllerChanged() override;
	virtual void OnRep_Controller() override;
	
	virtual void OnRep_PlayerState() override;

	
	//IPlayerInterface
	virtual bool GetCanPreInput_Implementation() override;
	virtual void SetCanPreInput_Implementation(const bool bPreInput) override;
	virtual void GetPreInputTag_Implementation(TArray<FGameplayTag>& OutPreInputTag) override;
	virtual bool FindActivatableAbilityTag_Implementation() override;
	virtual void ClearPreInputTag_Implementation() override;
	virtual FName GetLeftHandSocketName_Implementation() override;
	virtual FName GetRightHandSocketName_Implementation() override;
	virtual bool GetbIsLocking_Implementation() override;
	virtual AActor* GetLockingActor_Implementation() override;
	virtual EPlayerCategory GetPlayerCategory_Implementation() override;
	virtual void UpdateVisibleEnemyHealthBars_Implementation(TArray<AActor*>& OutVisibleEnemies) override;
	virtual void DetectInteractableActors_Implementation(TArray<AActor*>& OutInteractableActors) override;
	virtual AActor* GetCurrentInteractableActor_Implementation() override;
	virtual FText GetCurrentInteractableDisplayName_Implementation() override;
	virtual void InteractWithCurrentTarget_Implementation() override;



public:

	
	UPROPERTY()
	TObjectPtr<AProjectPlayerState> ProjectPlayerState;
	
	UPROPERTY()
	TObjectPtr<AProjectPlayerController> ProjectPlayerController;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="交互", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPlayerInteractionComponent> PlayerInteractionComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	float MaxSpringArmLength=500.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	float MinSpringArmLength=200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Position")
	EPlayerCategory PlayerCategory=EPlayerCategory::Empty;

	/** 玩家死亡后等待多久再显示复活界面。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="状态|死亡", meta=(ClampMin="0.0"))
	float DeathUIShowDelay = 1.0f;

	/** 当前交互对象变化时通知蓝图，可在此读取新对象的显示名称并刷新提示。 */
	UPROPERTY(BlueprintAssignable, Category="交互")
	FOnInteractableTargetChanged OnInteractableTargetChanged;

	/** 按滚轮方向切换当前可交互对象；正数为下一个，负数为上一个。 */
	void CycleInteractableTarget(int32 Direction);

	UFUNCTION(BlueprintPure, Category="交互")
	UPlayerInteractionComponent* GetPlayerInteractionComponent() const { return PlayerInteractionComponent; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	AProjectPlayerController* GetProjectPlayerController();
	UFUNCTION(BlueprintCallable,BlueprintPure)
	AProjectPlayerState* GetProjectPlayerState();

private:
	UFUNCTION()
	void HandleInteractionTargetChanged(AActor* PreviousTarget, AActor* NewTarget);
	UFUNCTION(Server, Reliable)
	void Server_InteractWithTarget(AActor* Target);
	void ScheduleDeathWidget();
	void ShowDeathWidget();
	FTimerHandle DeathUIShowTimerHandle;
};
