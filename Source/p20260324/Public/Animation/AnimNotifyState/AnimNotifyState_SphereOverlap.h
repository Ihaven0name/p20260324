#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SphereTrace.generated.h"

class AProjectBaseCharacter;

UCLASS()
class P20260324_API UAnimNotifyState_SphereTrace : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	TObjectPtr<AProjectBaseCharacter> OwnerCharacter;
	UPROPERTY()
	TSet<AActor*> OverlappingActors;



	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FName> SocketNamArray;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float OverlapRadius=10.f;
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;
};
