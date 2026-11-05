#include "Animation/AnimNotifyState/AnimNotifyState_AddStateTag.h"

#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotifyState_AddStateTag::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner, AddStateTags);
	}
}

void UAnimNotifyState_AddStateTag::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, AddStateTags);
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
