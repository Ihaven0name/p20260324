#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SphereOverlap.generated.h"

class AProjectBaseCharacter;

UCLASS()
class P20260324_API UAnimNotifyState_SphereOverlap : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	/** 已碰撞过的 Actor（去重用），每帧检测到新 Actor 立即造成伤害后加入此集合 */
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphereOverlap")
	TArray<FName> SocketNameArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphereOverlap")
	float OverlapRadius = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "SphereOverlap")
	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphereOverlap | Ability")
	FGameplayTag AbilityTag;
};
