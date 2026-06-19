#include "ProjectGameInstance.h"

#include "GAS/ProjectGameplayTag.h"

FString UProjectGameInstance::GetSaveSlotName(const EProjectSaveSlotName InSaveSlotName)
{
	switch (InSaveSlotName)
	{
	case EProjectSaveSlotName::MappableKey:
		return TEXT("MappableKeySlot");
	default:
		return TEXT("UnknownSaveSlot");
	}
}

void UProjectGameInstance::Init()
{
	Super::Init();

	const FProjectGameplayTag& GameplayTags = FProjectGameplayTag::Get();

	XPTagToXPValue.Empty();
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Large, 2000);
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Medium, 500);
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Small, 200);
}
