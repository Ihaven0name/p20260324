// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Defense.generated.h"

/**
 * UMMC_Defense
 * 
 * 两层查表：PlayerCategory → CurveTable → AttributeRowName → 曲线值
 */
UCLASS(Blueprintable)
class P20260324_API UMMC_Defense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_Defense();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName PlayerCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName AttributeRowName;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition LevelCaptureDef;
};
