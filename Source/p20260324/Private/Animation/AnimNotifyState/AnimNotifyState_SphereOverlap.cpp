#include "Animation/AnimNotifyState/AnimNotifyState_SphereOverlap.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GAS/GA/AbilityBase_Attack.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

void UAnimNotifyState_SphereOverlap::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	AlreadyHitActors.Empty();
}

void UAnimNotifyState_SphereOverlap::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_SphereOverlap::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!MeshComp || !AbilityTag.IsValid()) return;

	// （Listen Server 上两者可能指向不同网络角色视图）
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner||!Owner->GetInstigatorController()) return;
	if (!Owner->Implements<UCharacterInterface>()) return;

	USkeletalMeshComponent* ActiveMesh = MeshComp;

	UProjectAbilitySystemComponent* ASC = ICharacterInterface::Execute_GetAbilitySystemComponentByInterface(Owner);
	if (!ASC) return;

	FGameplayAbilitySpec MatchingSpec;
	if (!ASC->GetAbilitySpecByAbilityTag(AbilityTag, MatchingSpec)) return;

	UAbilityBase_Attack* AttackAbility = nullptr;
	for (UGameplayAbility* Instance : MatchingSpec.GetAbilityInstances())
	{
		AttackAbility = Cast<UAbilityBase_Attack>(Instance);
		if (AttackAbility) break;
	}
	if (!AttackAbility) return;

	for (const FName& SocketName : SocketNameArray)
	{
		if (!ActiveMesh || !ActiveMesh->DoesSocketExist(SocketName))
		{
			continue;
		}

		FVector SocketLocation = ActiveMesh->GetSocketLocation(SocketName);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Owner);

		for (const TWeakObjectPtr<AActor>& WeakActor : AlreadyHitActors)
		{
			if (WeakActor.IsValid())
			{
				ActorsToIgnore.Add(WeakActor.Get());
			}
		}

		TArray<AActor*> TickOverlappingActors;
		UKismetSystemLibrary::SphereOverlapActors(
			Owner->GetWorld(),
			SocketLocation,
			OverlapRadius,
			OverlapObjectTypes,
			AActor::StaticClass(),
			ActorsToIgnore,
			TickOverlappingActors
		);

		for (AActor* HitActor : TickOverlappingActors)
		{
			if (!IsValid(HitActor)) continue;
			AlreadyHitActors.Add(HitActor);
			if (AttackAbility->bOverrideKnockBack)
			{
				FDamageEffectParam DamageParam = AttackAbility->GetDefaultDamageEffectParam(HitActor, AttackAbility->bOverrideKnockBack, AttackAbility->KnockBackPitchOverride);
				UProjectBlueprintFunctionLibrary::ApplyDamageGameplayEffectByDamageEffectParam(DamageParam);
			}
			else
			{
				FDamageEffectParam DamageParam = AttackAbility->GetDefaultDamageEffectParam(HitActor);
				UProjectBlueprintFunctionLibrary::ApplyDamageGameplayEffectByDamageEffectParam(DamageParam);
			}
		}

		if (Owner->HasAuthority())
		{
			DrawDebugSphere(Owner->GetWorld(), SocketLocation, OverlapRadius, 16, FColor::Red, false, 5.f);
		}
	}
}
