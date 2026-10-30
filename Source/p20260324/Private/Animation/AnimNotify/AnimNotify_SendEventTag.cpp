#include "Animation/AnimNotify/AnimNotify_SendEventTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "GAS/ProjectGameplayTag.h"

void UAnimNotify_SendEventTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !EventTag.IsValid()) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (!ASC) return;
	if (EventTag == FProjectGameplayTag::Get().Event_Crouch_Dash_AddLaunch)
	{
		const APawn* OwnerPawn = Cast<APawn>(OwnerActor);
		UE_LOG(LogTemp, Warning,
			TEXT("[DashDiag] NotifySend Actor=%s Mesh=%s NetMode=%d LocalRole=%d LocalControlled=%d EventTag=%s"),
			*GetNameSafe(OwnerActor), *GetNameSafe(MeshComp),
			static_cast<int32>(OwnerActor->GetNetMode()),
			static_cast<int32>(OwnerActor->GetLocalRole()), OwnerPawn ? OwnerPawn->IsLocallyControlled() : 0,
			*EventTag.ToString());
	}
	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	Payload.Instigator = OwnerActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, EventTag, Payload);
}

