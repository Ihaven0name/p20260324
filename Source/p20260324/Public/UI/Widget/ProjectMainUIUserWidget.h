#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Overlay.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "ProjectMainUIUserWidget.generated.h"


struct FGameplayTag;

UCLASS()
class P20260324_API UProjectMainUIUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> Stack_Gameplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> Stack_Prompt;

};
