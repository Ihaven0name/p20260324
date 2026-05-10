#include "Animation/AnimNotify/AnimNotify_OpenTempInputWindow.h"

#include "Interface/CharacterInterface.h"

void UAnimNotify_OpenTempInputWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (Owner->Implements<UCharacterInterface>())
	{
		ICharacterInterface::Execute_AddInputWindowTimer(Owner,IntervalTime,OwnedTags);
	}
}
