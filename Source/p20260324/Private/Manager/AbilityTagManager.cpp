#include "Manager/AbilityTagManager.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Blueprint.h"
#include "GAS/GA/GameplayAbilityBase.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"

TObjectPtr<UDataTable> UAbilityTagManager::AbilityTagConfigDataTable;
void UAbilityTagManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	AbilityTagConfigDataTable=LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprint/DataTable/DT_AbilityTagConfig.DT_AbilityTagConfig"));
}

void UAbilityTagManager::Deinitialize()
{
	Super::Deinitialize();
}

void UAbilityTagManager::RefreshSingleAbilityTagInCDO(FName ChangedRowName)
{
	if (ChangedRowName.IsNone())
	{
		return;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TSet<FTopLevelAssetPath> DerivedClassNames;
	{
		TArray<FTopLevelAssetPath> BaseClassNames;
		BaseClassNames.Add(UGameplayAbilityBase::StaticClass()->GetClassPathName());

		TSet<FTopLevelAssetPath> ExcludedClassNames;
		AssetRegistry.GetDerivedClassNames(BaseClassNames, ExcludedClassNames, DerivedClassNames);
	}

	FARFilter Filter;
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray<FAssetData> BlueprintAssets;
	AssetRegistry.GetAssets(Filter, BlueprintAssets);

	for (const FAssetData& Asset : BlueprintAssets)
	{
		const FAssetTagValueRef GeneratedClassTag = Asset.TagsAndValues.FindTag(TEXT("GeneratedClass"));
		if (!GeneratedClassTag.IsSet())
		{
			continue;
		}

		const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(GeneratedClassTag.GetValue());
		if (ClassObjectPath.IsEmpty())
		{
			continue;
		}

		const FTopLevelAssetPath ClassPath(ClassObjectPath);
		if (!DerivedClassNames.Contains(ClassPath))
		{
			continue;
		}

		UBlueprint* Blueprint = Cast<UBlueprint>(Asset.GetAsset());
		if (!Blueprint)
		{
			continue;
		}

		UClass* Class = LoadObject<UClass>(nullptr, *ClassObjectPath);
		if (!Class || Class->HasAnyClassFlags(CLASS_Abstract))
		{
			continue;
		}

		UGameplayAbilityBase* CDO = Cast<UGameplayAbilityBase>(Class->GetDefaultObject());
		if (!CDO)
		{
			continue;
		}

		if (CDO->AbilityTagName != ChangedRowName)
		{
			continue;
		}

		Blueprint->Modify();
		CDO->Modify();
		CDO->InitializeTags();
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
		Blueprint->MarkPackageDirty();
		break;
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
