#include "Animation/AnimNotifyState/AnimNotifyState_InputWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAnimNotifyState_InputWindow::NotifyBegin(
	USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	FGameplayTagContainer ExistingWindowTags;
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		for (const FGameplayTag& WindowTag : WindowTags)
		{
			if (ASC->GetTagCount(WindowTag) > 0)
			{
				ExistingWindowTags.AddTag(WindowTag);
			}
		}
	}
	if (!ExistingWindowTags.IsEmpty())
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, ExistingWindowTags, Owner->HasAuthority());
	}
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner, WindowTags, Owner->HasAuthority());
}

void UAnimNotifyState_InputWindow::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	FGameplayTagContainer ExistingWindowTags;
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		for (const FGameplayTag& WindowTag : WindowTags)
		{
			if (ASC->GetTagCount(WindowTag) > 0)
			{
				ExistingWindowTags.AddTag(WindowTag);
			}
		}
	}
	if (!ExistingWindowTags.IsEmpty())
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, ExistingWindowTags, Owner->HasAuthority());
	}
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
