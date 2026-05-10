// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/MMC/MMC_AttackPower.h"
#include "Manager/ConfigManager.h"
#include "GAS/ProjectAttributeSet.h"

UMMC_AttackPower::UMMC_AttackPower()
{
	PlayerCategory = TEXT("Empty");
	AttributeRowName = TEXT("AttackPower");

	LevelCaptureDef.AttributeToCapture = UProjectAttributeSet::GetLevelAttribute();
	LevelCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LevelCaptureDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(LevelCaptureDef);
}

float UMMC_AttackPower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float Level = 0.f;
	GetCapturedAttributeMagnitude(LevelCaptureDef, Spec, EvaluateParams, Level);

	if (Level <= 0.f) return 0.f;

	AActor* AvatarActor = Cast<AActor>(Spec.GetEffectContext().GetSourceObject());
	if (!AvatarActor) return 0.f;
	UWorld* World = AvatarActor->GetWorld();
	if (!World) return 0.f;

	UConfigManager* CM = World->GetGameInstance()->GetSubsystem<UConfigManager>();
	if (!CM) return 0.f;

	UCurveTable* CurveTable = CM->GetCurveTableByRowName(PlayerCategory);
	if (!CurveTable) return 0.f;

	FRealCurve* RealCurve = CurveTable->FindCurve(AttributeRowName, TEXT("UMMC_AttackPower"));
	if (!RealCurve) return 0.f;

	return RealCurve->Eval(Level);
}
