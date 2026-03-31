#include "ProjectBaseCharacter.h"

#include "GAS/ProjectAbilitySystemComponent.h"

AProjectBaseCharacter::AProjectBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AProjectBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UProjectAbilitySystemComponent* AProjectBaseCharacter::GetAbilitySystemComponent()
{
	return AbilitySystemComponent;
}
