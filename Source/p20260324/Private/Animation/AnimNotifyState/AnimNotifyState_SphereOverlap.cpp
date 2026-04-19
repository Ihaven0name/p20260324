#include "Animation/AnimNotifyState/AnimNotifyState_SphereTrace.h"

#include "ProjectBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


void UAnimNotifyState_SphereTrace::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (AProjectBaseCharacter* ProjectBaseCharacter = Cast<AProjectBaseCharacter>(Owner))
	{
		OwnerCharacter=ProjectBaseCharacter;
	}
	OverlappingActors.Empty();
}

void UAnimNotifyState_SphereTrace::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
}

void UAnimNotifyState_SphereTrace::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	for (const FName& SocketName:SocketNamArray)
	{
		if (!OwnerCharacter->GetMesh()->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp,Warning,TEXT("In ANS_SphereTrace Have No SocketName"));
		}
		FVector SocketLocation=OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		TArray<AActor*> TickOverlappingActors;
		UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			SocketLocation,
			OverlapRadius,
			OverlapObjectTypes,
			AActor::StaticClass(),
			ActorsToIgnore,
			TickOverlappingActors
		);
		for (AActor*& TempActor:TickOverlappingActors)
		{
			OverlappingActors.Add(TempActor);
		}
	}
	
}
