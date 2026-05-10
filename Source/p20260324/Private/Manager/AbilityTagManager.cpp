#include "Manager/AbilityTagManager.h"

#include "GAS/GA/GameplayAbilityBase.h"

TObjectPtr<UDataTable> UAbilityTagManager::AbilityTagConfigDataTable;
void UAbilityTagManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	AbilityTagConfigDataTable=LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprint/DataTable/DT_AbilityTagConfig.DT_AbilityTagConfig"));
	AbilityTagConfigDataTable->OnDataTableChanged().AddUObject(this,&ThisClass::RefreshAbilityTagInCDO);
}

void UAbilityTagManager::Deinitialize()
{
	AbilityTagConfigDataTable->OnDataTableChanged().RemoveAll(this);
	Super::Deinitialize();
}

void UAbilityTagManager::RefreshAbilityTagInCDO()
{
	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if (!Class)
		{
			continue;
		}

		if (!Class->IsChildOf(UGameplayAbilityBase::StaticClass()))
		{
			continue;
		}

		if (Class->HasAnyClassFlags(CLASS_Abstract))
		{
			continue;
		}

		UGameplayAbilityBase* CDO = Cast<UGameplayAbilityBase>(Class->GetDefaultObject());
		if (!CDO)
		{
			continue;
		}

		CDO->InitializeTags();
	}
}
FAbilityTagConfig* UAbilityTagManager::FindAbilityTagConfigByRowName(FName RowName)
{
	if (IsValid(AbilityTagConfigDataTable))
	{
		return AbilityTagConfigDataTable->FindRow<FAbilityTagConfig>(RowName, nullptr);
	}
	return nullptr;
}
