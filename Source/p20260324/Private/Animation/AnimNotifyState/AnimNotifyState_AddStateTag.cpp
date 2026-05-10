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
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(),AddStateTags);
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(),AddStateTags);
}

void UAnimNotifyState_AddStateTag::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(),AddStateTags);
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
