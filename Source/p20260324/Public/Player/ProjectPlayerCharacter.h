



#pragma once

#include "CoreMinimal.h"
#include "ProjectBaseCharacter.h"
#include "Interface/PlayerInterface.h"
#include "ProjectPlayerCharacter.generated.h"


class AProjectPlayerController;
class AProjectPlayerState;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class P20260324_API AProjectPlayerCharacter : public AProjectBaseCharacter , public IPlayerInterface
{
	GENERATED_BODY()
public:
	
	AProjectPlayerCharacter();

	//Self
	virtual void InitAbilityActorInfo() override;
protected:
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



public:

	
	UPROPERTY()
	TObjectPtr<AProjectPlayerState> ProjectPlayerState;
	
	UPROPERTY()
	TObjectPtr<AProjectPlayerController> ProjectPlayerController;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	float MaxSpringArmLength=500.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	float MinSpringArmLength=200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Position")
	EPlayerCategory PlayerCategory=EPlayerCategory::Empty;


	UFUNCTION(BlueprintCallable,BlueprintPure)
	AProjectPlayerController* GetProjectPlayerController();
	UFUNCTION(BlueprintCallable,BlueprintPure)
	AProjectPlayerState* GetProjectPlayerState();
};
