#include "GAS/MMC/MMC_EnemyDefense.h"

#include "GAS/ProjectAttributeSet.h"
#include "Manager/ConfigManager.h"

UMMC_EnemyDefense::UMMC_EnemyDefense()
{
	AttributeRowName = TEXT("Defense");

	LevelCaptureDef.AttributeToCapture = UProjectAttributeSet::GetLevelAttribute();
	LevelCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LevelCaptureDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(LevelCaptureDef);
}

float UMMC_EnemyDefense::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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
	if (!World || !World->GetGameInstance()) return 0.f;

	UConfigManager* ConfigManager = World->GetGameInstance()->GetSubsystem<UConfigManager>();
	if (!ConfigManager) return 0.f;

	const FString CategoryName = StaticEnum<EEnemyCategory>()->GetNameStringByValue(static_cast<int64>(EnemyCategory));
	UCurveTable* CurveTable = ConfigManager->GetEnemyCurveTableByRowName(FName(*CategoryName));
	if (!CurveTable) return 0.f;

	const FRealCurve* RealCurve = CurveTable->FindCurve(AttributeRowName, TEXT("UMMC_EnemyDefense"));
	return RealCurve ? RealCurve->Eval(Level) : 0.f;
}
