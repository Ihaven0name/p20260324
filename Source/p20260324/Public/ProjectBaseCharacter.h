
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UProjectAbilitySystemComponent;

UCLASS()
class P20260324_API AProjectBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectBaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;




public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;
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



	
	UFUNCTION(BlueprintCallable)
	UProjectAbilitySystemComponent* GetAbilitySystemComponent();

};
