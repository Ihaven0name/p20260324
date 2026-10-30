#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Info/CharacterInfo.h"
#include "MMC_EnemyAttackPower.generated.h"

UCLASS(Blueprintable)
class P20260324_API UMMC_EnemyAttackPower : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_EnemyAttackPower();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	EEnemyCategory EnemyCategory = EEnemyCategory::Empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName AttributeRowName;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition LevelCaptureDef;
};
