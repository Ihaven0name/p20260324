#include "Player/ProjectPlayerState.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/ConfigManager.h"
#include "Net/UnrealNetwork.h"
#include "Player/ProjectPlayerCharacter.h"
#include "Info/InventoryItemInfo.h"
#include "Manager/InventoryManager.h"
#include "Info/InventoryInfo.h"
#include "GAS/ProjectGameplayTag.h"
#include "Net/Core/PushModel/PushModel.h"
class UCharacterInfo;

AProjectPlayerState::AProjectPlayerState()
{
	
}


void AProjectPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 同步数组给所有客户端
	
	DOREPLIFETIME(AProjectPlayerState, ControlledCharacterIndex);
	DOREPLIFETIME(AProjectPlayerState, TeamArray);
	DOREPLIFETIME_CONDITION(AProjectPlayerState, InventoryItems, COND_OwnerOnly);
}

int32 AProjectPlayerState::GetInventoryItemQuantity(const FGameplayTag ItemTag) const
{
	const FInventoryItem* FoundItem = InventoryItems.FindByPredicate(
		[&ItemTag](const FInventoryItem& Item) { return Item.ItemTag == ItemTag; });
	return FoundItem ? FoundItem->ItemQuantity : 0;
}

bool AProjectPlayerState::AddInventoryItem(const FGameplayTag ItemTag, const int32 Count)
{
	if (!HasAuthority() || !ItemTag.IsValid() || Count <= 0)
	{
		return false;
	}

	UConfigManager* ConfigManager = GetGameInstance() ? GetGameInstance()->GetSubsystem<UConfigManager>() : nullptr;
	UInventoryInfo* InventoryInfo = ConfigManager ? ConfigManager->GetInventoryInfo() : nullptr;
	UInventoryItemInfo* ItemInfo = nullptr;
	if (!InventoryInfo || !InventoryInfo->FindInventoryItemInfoByItemTag(ItemTag, ItemInfo) || !ItemInfo)
	{
		return false;
	}

	FInventoryItem* ExistingItem = InventoryItems.FindByPredicate(
		[&ItemTag](const FInventoryItem& Item) { return Item.ItemTag == ItemTag; });
	const int32 ExistingQuantity = ExistingItem ? ExistingItem->ItemQuantity : 0;
	const int32 NewQuantity = FMath::Min(ExistingQuantity + Count, ItemInfo->MaxStackNumber);
	if (NewQuantity <= ExistingQuantity)
	{
		return false;
	}

	if (ExistingItem)
	{
		ExistingItem->ItemQuantity = NewQuantity;
	}
	else
	{
		FInventoryItem& NewItem = InventoryItems.AddDefaulted_GetRef();
		NewItem.ItemTag = ItemTag;
		NewItem.ItemQuantity = NewQuantity;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(AProjectPlayerState, InventoryItems, this);
	ForceNetUpdate();
	OnRep_InventoryItems();
	return true;
}

bool AProjectPlayerState::ConsumeExperienceMaterials(
	const int32 SmallCount, const int32 MediumCount, const int32 LargeCount)
{
	if (!HasAuthority() || SmallCount < 0 || MediumCount < 0 || LargeCount < 0 ||
		(SmallCount == 0 && MediumCount == 0 && LargeCount == 0))
	{
		return false;
	}

	const FProjectGameplayTag& GameplayTags = FProjectGameplayTag::Get();
	const FGameplayTag RequiredTags[] = {
		GameplayTags.Inventory_Material_Experience_Small,
		GameplayTags.Inventory_Material_Experience_Medium,
		GameplayTags.Inventory_Material_Experience_Large
	};
	const int32 RequiredCounts[] = { SmallCount, MediumCount, LargeCount };
	for (int32 Index = 0; Index < UE_ARRAY_COUNT(RequiredTags); ++Index)
	{
		if (GetInventoryItemQuantity(RequiredTags[Index]) < RequiredCounts[Index])
		{
			return false;
		}
	}

	for (int32 Index = 0; Index < UE_ARRAY_COUNT(RequiredTags); ++Index)
	{
		if (RequiredCounts[Index] <= 0)
		{
			continue;
		}
		const int32 NewQuantity = GetInventoryItemQuantity(RequiredTags[Index]) - RequiredCounts[Index];
		if (NewQuantity == 0)
		{
			InventoryItems.RemoveAll([&RequiredTags, Index](const FInventoryItem& Item)
			{
				return Item.ItemTag == RequiredTags[Index];
			});
		}
		else if (FInventoryItem* Item = InventoryItems.FindByPredicate([&RequiredTags, Index](const FInventoryItem& Candidate)
			{ return Candidate.ItemTag == RequiredTags[Index]; }))
		{
			Item->ItemQuantity = NewQuantity;
		}
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(AProjectPlayerState, InventoryItems, this);
	ForceNetUpdate();
	OnRep_InventoryItems();
	return true;
}

void AProjectPlayerState::OnRep_InventoryItems()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UInventoryManager* InventoryManager = GameInstance->GetSubsystem<UInventoryManager>())
		{
			InventoryManager->SyncFromAuthoritativeInventory(InventoryItems);
		}
	}
}

AProjectPlayerCharacter* AProjectPlayerState::AddTeamInfo(FGameplayTag CharacterTag, FVector Location, FRotator Rotation)
{

	if (!HasAuthority())
	{
		return nullptr;
	}
	if (TeamArray.Num() >= TeamBound)
	{
		return nullptr;
	}
	for (FTeamCharacterInfo& TempTeamCharacterInfo : TeamArray)
	{
		if (TempTeamCharacterInfo.CharacterTag.MatchesTagExact(CharacterTag))
		{
			return nullptr;
		}
	}
	UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(this);
	UCharacterInfo* CharacterInfo = ConfigManager->GetCharacterInfo();
	UTeamInfo* TeamInfo = ConfigManager->GetTeamInfo();
	TSubclassOf<AProjectPlayerCharacter> CharacterClass;
	if (!CharacterInfo->FindCharacterClassByTag(CharacterTag,CharacterClass))
	{
		return nullptr;
	}
	FActorSpawnParameters Params;
	Params.Owner = GetPlayerController();

	if (AProjectPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AProjectPlayerCharacter>(CharacterClass, Location, Rotation, Params))
	{
		NewCharacter->SetOwner(GetPlayerController());
		NewCharacter->SetPlayerState(this);
		NewCharacter->InitAbilityActorInfo();
		
		FTeamCharacterInfo NewInfo;
		NewInfo.CharacterTag = CharacterTag;
		NewInfo.Character = NewCharacter;
		NewInfo.AbilitySystemComponent = NewCharacter->GetAbilitySystemComponent();
		NewInfo.AttributeSet = NewCharacter->GetAttributeSet();
		TeamInfo->FindTeamStaticInfoByTag(CharacterTag,NewInfo.TeamStaticInfo);
        
		TeamArray.Add(NewInfo);
        
		// 7. 强制同步（如果是手动修改数组，有时需要调用这个）
		MARK_PROPERTY_DIRTY_FROM_NAME(AProjectPlayerState, TeamArray, this);
		return NewCharacter;
	}
	return nullptr;
}

bool AProjectPlayerState::ReplaceTeamCharacter(AProjectPlayerCharacter* OldCharacter, AProjectPlayerCharacter* NewCharacter)
{
	if (!HasAuthority() || !IsValid(OldCharacter) || !IsValid(NewCharacter))
	{
		return false;
	}

	for (int32 Index = 0; Index < TeamArray.Num(); ++Index)
	{
		FTeamCharacterInfo& TeamCharacter = TeamArray[Index];
		if (TeamCharacter.Character == OldCharacter)
		{
			TeamCharacter.Character = NewCharacter;
			TeamCharacter.AbilitySystemComponent = NewCharacter->GetAbilitySystemComponent();
			TeamCharacter.AttributeSet = NewCharacter->GetAttributeSet();
			ControlledCharacterIndex = Index;
			MARK_PROPERTY_DIRTY_FROM_NAME(AProjectPlayerState, TeamArray, this);
			MARK_PROPERTY_DIRTY_FROM_NAME(AProjectPlayerState, ControlledCharacterIndex, this);
			ForceNetUpdate();
			return true;
		}
	}

	return false;
}
