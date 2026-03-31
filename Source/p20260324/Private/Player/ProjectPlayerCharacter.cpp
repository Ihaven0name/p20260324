
#include "Player/ProjectPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AProjectPlayerCharacter::AProjectPlayerCharacter()
{
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength=500.f;
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bUsePawnControlRotation=true;
	SpringArmComponent->bInheritPitch=true;
	SpringArmComponent->bInheritRoll=true;
	SpringArmComponent->bInheritYaw=true;
	
	FRotator Rotator=FRotator(-25.0f,0.f,0.0f);
	SpringArmComponent->SetRelativeRotation(Rotator);
	
	CameraComponent=CreateDefaultSubobject<UCameraComponent>("CameraComponent");	
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation=false;

	
	
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bOrientRotationToMovement=true;
	CharacterMovementComponent->RotationRate=FRotator(0.f, 400.f, 0.f);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;
	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
}
