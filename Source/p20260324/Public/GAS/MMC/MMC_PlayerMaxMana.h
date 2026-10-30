// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Info/CharacterInfo.h"
#include "MMC_PlayerMaxMana.generated.h"

/**
 * UMMC_PlayerMaxMana
 * 
 * 两层查表：PlayerCategory → CurveTable → AttributeRowName → 曲线值
 */
UCLASS(Blueprintable)
class P20260324_API UMMC_PlayerMaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_PlayerMaxMana();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	EPlayerCategory PlayerCategory = EPlayerCategory::Empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName AttributeRowName;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition LevelCaptureDef;
};
