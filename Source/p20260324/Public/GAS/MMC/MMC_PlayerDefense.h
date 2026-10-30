// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Info/CharacterInfo.h"
#include "MMC_PlayerDefense.generated.h"

/**
 * UMMC_PlayerDefense
 * 
 * 两层查表：PlayerCategory → CurveTable → AttributeRowName → 曲线值
 */
UCLASS(Blueprintable)
class P20260324_API UMMC_PlayerDefense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_PlayerDefense();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	EPlayerCategory PlayerCategory = EPlayerCategory::Empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName AttributeRowName;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition LevelCaptureDef;
};
