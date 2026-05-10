#include "Animation/AnimNotifyState/AnimNotifyState_InputWindow.h"

#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotifyState_InputWindow::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner||!Owner->GetInstigatorController()) return;
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner,WindowTags);
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner,WindowTags);
}

void UAnimNotifyState_InputWindow::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner||!Owner->GetInstigatorController()) return;
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner,WindowTags);
	UE_LOG(LogTemp,Warning,TEXT("UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags:%s"),*Animation->GetName());
}

void UAnimNotifyState_InputWindow::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}
