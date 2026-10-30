#include "Manager/AbilityTagManager.h"

#include "Engine/DataTable.h"
#include "Info/AbilityInfo.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Blueprint.h"
#include "GAS/GA/GameplayAbilityBase.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#endif

const FAbilityTagConfig* FAbilityTagManager::FindAbilityTagConfigByRowName(const FName RowName)
{
	UDataTable* AbilityTagConfigDataTable = LoadObject<UDataTable>(
		nullptr,
		TEXT("/Game/Blueprint/DataTable/DT_AbilityTagConfig.DT_AbilityTagConfig"));
	if (!IsValid(AbilityTagConfigDataTable))
	{
		return nullptr;
	}

	return AbilityTagConfigDataTable->FindRow<FAbilityTagConfig>(RowName, nullptr);
}

#if WITH_EDITOR
void FAbilityTagManager::RefreshSingleAbilityTagInCDO(const FName ChangedRowName)
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
		if (!CDO || CDO->AbilityTagName != ChangedRowName)
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
#endif
