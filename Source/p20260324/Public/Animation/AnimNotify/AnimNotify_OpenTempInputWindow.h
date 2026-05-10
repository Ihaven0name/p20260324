#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_OpenTempInputWindow.generated.h"

UCLASS()
class P20260324_API UAnimNotify_OpenTempInputWindow : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float IntervalTime=0.5f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTagContainer OwnedTags;
};
