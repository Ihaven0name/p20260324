#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Framework/Commands/InputChord.h"
#include "MultiControlWidgetController.h"
#include "SettingUIWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingSwitcherIndexChangedSignature, int32, InIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingMappableKeyClearedSignature, FGameplayTag, GroupTag, EPlayerMappableKeySlot, Slot);



UCLASS(Blueprintable)
class P20260324_API USettingUIWidgetController : public UMultiControlWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallback() override;
	virtual void BroadcastInitialAttribute() const override;

	UPROPERTY(BlueprintAssignable, Category = "Setting")
	FOnSettingSwitcherIndexChangedSignature OnSettingSwitcherIndexChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "Setting")
	FOnSettingMappableKeyClearedSignature OnSettingMappableKeyClearedSignature;

	UFUNCTION(BlueprintCallable, Category = "Setting|Mappable")
	bool RemapMappableOneSlotKeyByGroupTag(FGameplayTag GroupTag, const FInputChord& SelectedInputChord);

	UFUNCTION(BlueprintCallable, Category = "Setting|Mappable")
	bool RemapMappableKeyByGroupTagWithSlot(FGameplayTag GroupTag, EPlayerMappableKeySlot InSlot, const FInputChord& SelectedInputChord);

	UFUNCTION(BlueprintCallable, Category = "Setting|Mappable")
	bool ClearMappableKeyByGroupTagWithSlot(FGameplayTag GroupTag, EPlayerMappableKeySlot InSlot);
};
