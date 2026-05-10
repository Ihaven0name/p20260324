#include "Animation/AnimNotifyState/AnimNotifyState_PreInputWindow.h"

#include "Interface/PlayerInterface.h"

void UAnimNotifyState_PreInputWindow::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner||!Owner->GetInstigatorController()) return;
	if (Owner&&Owner->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_ClearPreInputTag(Owner);
		IPlayerInterface::Execute_SetCanPreInput(Owner,true);
	}
}

void UAnimNotifyState_PreInputWindow::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner||!Owner->GetInstigatorController()) return;
	if (Owner&&Owner->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetCanPreInput(Owner,false);
	}
}

void UAnimNotifyState_PreInputWindow::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}
