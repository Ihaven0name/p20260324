
#include "Manager/ProjectAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "GAS/ProjectGameplayTag.h"

void UProjectAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FProjectGameplayTag::InitGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

UProjectAssetManager& UProjectAssetManager::Get()
{
	check(GEngine);
	UProjectAssetManager* AuroAssetManager = Cast<UProjectAssetManager>(GEngine->AssetManager);
	return *AuroAssetManager;
}
