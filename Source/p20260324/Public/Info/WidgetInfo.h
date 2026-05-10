
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WidgetInfo.generated.h"

class UCommonActivatableWidget;
class UProjectWidgetController;
class UProjectMainUIUserWidget;


USTRUCT(BlueprintType)
struct FWidgetProperty
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FGameplayTag StackLayerTag;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonActivatableWidget> WidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectWidgetController> WidgetControllerClass;
	
};

UCLASS()
class P20260324_API UWidgetInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag,FWidgetProperty> WidgetTagToWidgetProperty;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UProjectMainUIUserWidget> MainUIClass;

	UFUNCTION(BlueprintCallable)
	bool FindWidgetProperty(const FGameplayTag WidgetTag,FWidgetProperty& OutWidgetProperty);
};
