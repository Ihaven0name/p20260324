#include "Components/InteractableComponent.h"

#include "Interface/InteractableInterface.h"

bool UInteractableComponent::CanInteractWith(AActor* InteractingActor) const
{
	AActor* OwnerActor = GetOwner();
	return bCanInteract && IsValid(OwnerActor) && IsValid(InteractingActor) &&
		OwnerActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass());
}

bool UInteractableComponent::TryInteract(AActor* InteractingActor)
{
	AActor* OwnerActor = GetOwner();
	if (!CanInteractWith(InteractingActor))
	{
		return false;
	}

	IInteractableInterface::Execute_Interact(OwnerActor, InteractingActor);
	return true;
}
