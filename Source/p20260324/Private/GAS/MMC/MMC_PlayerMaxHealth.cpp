// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/MMC/MMC_PlayerMaxHealth.h"
#include "Manager/ConfigManager.h"
#include "GAS/ProjectAttributeSet.h"

UMMC_PlayerMaxHealth::UMMC_PlayerMaxHealth()
{
	AttributeRowName = TEXT("MaxHealth");

	// 定义并注册 Level 捕获（只做一次）
	LevelCaptureDef.AttributeToCapture = UProjectAttributeSet::GetLevelAttribute();
	LevelCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LevelCaptureDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(LevelCaptureDef);
}

float UMMC_PlayerMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	const FString CategoryName = StaticEnum<EPlayerCategory>()->GetNameStringByValue(static_cast<int64>(PlayerCategory));
	UCurveTable* CurveTable = CM->GetPlayerCurveTableByRowName(FName(*CategoryName));
	if (!CurveTable) return 0.f;

	FRealCurve* RealCurve = CurveTable->FindCurve(AttributeRowName, TEXT("UMMC_PlayerMaxHealth"));
	if (!RealCurve) return 0.f;

	return RealCurve->Eval(Level);
}
