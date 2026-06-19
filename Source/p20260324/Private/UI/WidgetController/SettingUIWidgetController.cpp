#include "UI/WidgetController/SettingUIWidgetController.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/InputManager.h"

void USettingUIWidgetController::BindCallback()
{
	Super::BindCallback();
}

void USettingUIWidgetController::BroadcastInitialAttribute() const
{
	
}

bool USettingUIWidgetController::RemapMappableOneSlotKeyByGroupTag(
	const FGameplayTag GroupTag,
	const FInputChord& SelectedInputChord)
{
	UInputManager* InputManager = UProjectBlueprintFunctionLibrary::GetInputManager(this);
	if (!InputManager || !GroupTag.IsValid() || !SelectedInputChord.Key.IsValid()) return false;

	return InputManager->RemapKeyByGroupTagOneSlot(GroupTag, SelectedInputChord);
}

bool USettingUIWidgetController::RemapMappableKeyByGroupTagWithSlot(
	const FGameplayTag GroupTag,
	const EPlayerMappableKeySlot InSlot,
	const FInputChord& SelectedInputChord)
{
	UInputManager* InputManager = UProjectBlueprintFunctionLibrary::GetInputManager(this);
	if (!InputManager || !GroupTag.IsValid() || !SelectedInputChord.Key.IsValid()) return false;

	bool bHasClearedDuplicateSlot = false;
	EPlayerMappableKeySlot ClearedSlot = EPlayerMappableKeySlot::Unspecified;
	if (!InputManager->RemapKeyByGroupTagWithSlotAndOutputClearedSlot(GroupTag, InSlot, SelectedInputChord, bHasClearedDuplicateSlot, ClearedSlot)) return false;

	if (bHasClearedDuplicateSlot && ClearedSlot != EPlayerMappableKeySlot::Unspecified)
		OnSettingMappableKeyClearedSignature.Broadcast(GroupTag, ClearedSlot);

	return true;
}

bool USettingUIWidgetController::ClearMappableKeyByGroupTagWithSlot(
	const FGameplayTag GroupTag,
	const EPlayerMappableKeySlot InSlot)
{
	UInputManager* InputManager = UProjectBlueprintFunctionLibrary::GetInputManager(this);
	if (!InputManager || !GroupTag.IsValid()) return false;
	if (!InputManager->ClearKeyByGroupTagWithSlot(GroupTag, InSlot)) return false;

	OnSettingMappableKeyClearedSignature.Broadcast(GroupTag, InSlot);
	return true;
}
