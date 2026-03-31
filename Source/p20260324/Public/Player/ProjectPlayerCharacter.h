
#pragma once

#include "CoreMinimal.h"
#include "ProjectBaseCharacter.h"
#include "ProjectPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

UCLASS()
class P20260324_API AProjectPlayerCharacter : public AProjectBaseCharacter
{
	GENERATED_BODY()
public:
	
	AProjectPlayerCharacter();

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	float MaxSpringArmLength=500.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	float MinSpringArmLength=200.f;
};
