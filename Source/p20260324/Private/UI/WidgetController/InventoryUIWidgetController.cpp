#include "UI/WidgetController/InventoryUIWidgetController.h"

#include "AbilitySystemComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Info/InventoryItemInfo.h"
#include "Manager/InventoryManager.h"
#include "UI/Item/ProjectInventoryItem.h"

namespace
{
	void BroadcastCharacterAttribute(
		const TArray<FWidgetControllerParam>& WidgetControllerParams,
		const FOnSpecCharacterAttributeChangeSignature& Signature,
		const TFunctionRef<float(const UProjectAttributeSet*)>& ValueGetter)
	{
		for (const FWidgetControllerParam& Param : WidgetControllerParams)
		{
			if (!Param.PlayerCharacter || !Param.AttributeSet)
			{
				continue;
			}

			Signature.Broadcast(Param.PlayerCharacter, ValueGetter(Param.AttributeSet));
		}
	}
}

void UInventoryUIWidgetController::BindCallback()
{
	Super::BindCallback();

	OnConsumableItemNumberChangedSignature.AddDynamic(this, &ThisClass::OnConsumableItemNumberChangedFunction);

	for (const FWidgetControllerParam& Param : WidgetControllerParams)
	{
		if (!Param.PlayerCharacter || !Param.AbilitySystemComponent || !Param.AttributeSet)
		{
			continue;
		}

		AProjectPlayerCharacter* PlayerCharacter = Param.PlayerCharacter;
		UProjectAbilitySystemComponent* AbilitySystemComponent = Param.AbilitySystemComponent;
		UProjectAttributeSet* AttributeSet = Param.AttributeSet;

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetHealthAttribute()).AddLambda(
			[this, PlayerCharacter, AttributeSet](const FOnAttributeChangeData&)
			{
				OnSpecCharacterHealthChangeSignature.Broadcast(PlayerCharacter, AttributeSet->GetHealth());
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetManaAttribute()).AddLambda(
			[this, PlayerCharacter, AttributeSet](const FOnAttributeChangeData&)
			{
				OnSpecCharacterManaChangeSignature.Broadcast(PlayerCharacter, AttributeSet->GetMana());
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxHealthAttribute()).AddLambda(
			[this, PlayerCharacter, AttributeSet](const FOnAttributeChangeData&)
			{
				OnSpecCharacterMaxHealthChangeSignature.Broadcast(PlayerCharacter, AttributeSet->GetMaxHealth());
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxManaAttribute()).AddLambda(
			[this, PlayerCharacter, AttributeSet](const FOnAttributeChangeData&)
			{
				OnSpecCharacterMaxManaChangeSignature.Broadcast(PlayerCharacter, AttributeSet->GetMaxMana());
			});
	}
}

void UInventoryUIWidgetController::ApplyConsumableToTeamCharacter(
	UProjectInventoryItem* SelectedItem,
	FTeamCharacterInfo& TeamCharacterInfo)
{
	UProjectAbilitySystemComponent* TempAbilitySystemComponent=TeamCharacterInfo.AbilitySystemComponent.Get();
	UProjectBlueprintFunctionLibrary::ApplyGameplayEffectSpecToSelfByClass(TempAbilitySystemComponent,SelectedItem->InventoryItemInfo->ConsumableEffect);
}

void UInventoryUIWidgetController::BroadcastCharacterHealthInitialValue()
{
	BroadcastCharacterAttribute(
		WidgetControllerParams,
		OnSpecCharacterHealthChangeSignature,
		[](const UProjectAttributeSet* AttributeSet)
		{
			return AttributeSet->GetHealth();
		});
}

void UInventoryUIWidgetController::BroadcastCharacterManaInitialValue()
{
	BroadcastCharacterAttribute(
		WidgetControllerParams,
		OnSpecCharacterManaChangeSignature,
		[](const UProjectAttributeSet* AttributeSet)
		{
			return AttributeSet->GetMana();
		});
}

void UInventoryUIWidgetController::BroadcastCharacterMaxHealthInitialValue()
{
	BroadcastCharacterAttribute(
		WidgetControllerParams,
		OnSpecCharacterMaxHealthChangeSignature,
		[](const UProjectAttributeSet* AttributeSet)
		{
			return AttributeSet->GetMaxHealth();
		});
}

void UInventoryUIWidgetController::BroadcastCharacterMaxManaInitialValue()
{
	BroadcastCharacterAttribute(
		WidgetControllerParams,
		OnSpecCharacterMaxManaChangeSignature,
		[](const UProjectAttributeSet* AttributeSet)
		{
			return AttributeSet->GetMaxMana();
		});
}

void UInventoryUIWidgetController::BroadcastSpecCharacterInitialValue(AProjectPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;
	const UProjectAttributeSet* AttributeSet = PlayerCharacter->GetAttributeSet();
	if (!AttributeSet) return;

	OnSpecCharacterHealthChangeSignature.Broadcast(PlayerCharacter, AttributeSet->GetHealth());
	OnSpecCharacterManaChangeSignature.Broadcast(PlayerCharacter, AttributeSet->GetMana());
}

void UInventoryUIWidgetController::OnConsumableItemNumberChangedFunction(const FGameplayTag ItemTag, int32 ChangedNumber)
{
	UInventoryManager* InventoryManager = UProjectBlueprintFunctionLibrary::GetInventoryManager(this);
	if (!InventoryManager || !ItemTag.IsValid() || ChangedNumber == 0)
	{
		return;
	}

	if (ChangedNumber > 0)
	{
		InventoryManager->AddItem(ItemTag, ChangedNumber);
	}
	else
	{
		InventoryManager->RemoveItem(ItemTag, FMath::Abs(ChangedNumber));
	}
}
