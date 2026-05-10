#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_LineTrace.generated.h"

UCLASS()
class P20260324_API UAnimNotifyState_LineTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	// ==========================================
	// 编辑器可配置参数
	// ==========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	FName StartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	FName EndSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	float TraceDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "LineTrace")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	float DetectionInterval = 0.f;

	// ==========================================
	// 关联 Ability Tag（用于 End 时查找对应 AbilitySpec）
	// ==========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace | Ability")
	FGameplayTag AbilityTag;

private:
	TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;
	float AccumulatedTime = 0.f;
};
