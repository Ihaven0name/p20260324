
#pragma once

#include "CoreMinimal.h"
#include "ProjectPlayerCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPlayerController.generated.h"


class AProjectPlayerCharacter;
class UProjectGameInstance;
struct FInputActionValue;
class UProjectAbilitySystemComponent;
class UInputAction;
class UInputMappingContext;

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

	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputAction")
	TObjectPtr<UInputAction> SpringArmScaleAction;
	


	UFUNCTION(BlueprintCallable)
	UProjectAbilitySystemComponent* GetAbilitySystemComponent();
	UFUNCTION(BlueprintCallable)
	UProjectGameInstance* GetProjectGameInstance();
	UFUNCTION(BlueprintCallable)
	AProjectPlayerCharacter* GetPlayerCharacter();
	UFUNCTION(BlueprintCallable)
	void MoveActionFunction(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void LookActionFunction(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void SpringArmScaleActionFunction(const FInputActionValue& Value);
};
