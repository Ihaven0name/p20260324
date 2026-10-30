// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/ProjectAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

void UProjectAnimInstance::StartTurnToTarget(const float SignedRotationSpeed)
{
	TurnToTargetRotationSpeed = SignedRotationSpeed;
	bIsTurningToTarget = true;
}

void UProjectAnimInstance::StopTurnToTarget()
{
	bIsTurningToTarget = false;
	TurnToTargetRotationSpeed = 0.0f;
}

UProjectAnimInstance* UProjectAnimInstance::GetMainAnimInstance(const UAnimInstance* LayerAnimInstance)
{
	if (!LayerAnimInstance)
	{
		return nullptr;
	}

	const USkeletalMeshComponent* MeshComponent = LayerAnimInstance->GetOwningComponent();
	if (!MeshComponent)
	{
		return nullptr;
	}

	return Cast<UProjectAnimInstance>(MeshComponent->GetAnimInstance());
}
