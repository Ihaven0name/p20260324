#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Info/CharacterInfo.h"
#include "MMC_EnemyMaxHealth.generated.h"

UCLASS(Blueprintable)
class P20260324_API UMMC_EnemyMaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_EnemyMaxHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	EEnemyCategory EnemyCategory = EEnemyCategory::Empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName AttributeRowName;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition LevelCaptureDef;
};
