#include "ProjectGameInstance.h"

#include "GAS/ProjectGameplayTag.h"

void UProjectGameInstance::Init()
{
	Super::Init();

	const FProjectGameplayTag& GameplayTags = FProjectGameplayTag::Get();

	XPTagToXPValue.Empty();
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Large, 2000);
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Medium, 500);
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Small, 200);
}
