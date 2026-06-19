#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Input/MappingBehavior.h"
#include "MappableInfo.generated.h"


//一个按键可能对应很多个InputAction，因为打开UI和关闭UI一般是一样的UI
USTRUCT(BlueprintType)
struct FMappableActionGroup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag GroupTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UMappingBehavior>> MappableBehaviors;
};



UCLASS()
class P20260324_API UMappableInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mappable")
	TArray<FMappableActionGroup> MappableActionGroups;
	
};
