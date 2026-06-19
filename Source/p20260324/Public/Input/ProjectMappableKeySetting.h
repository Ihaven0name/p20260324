#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ProjectMappableKeySetting.generated.h"


USTRUCT(BlueprintType)
struct FMapPlayerOperationArgs
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	FName BehaviorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	EPlayerMappableKeySlot Slot = EPlayerMappableKeySlot::First;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	FKey NewKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	bool bShift = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	bool bControl = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	bool bAlt = false;
};

USTRUCT(BlueprintType)
struct FTaggedMapPlayerOperationArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	FGameplayTag IMCTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|User Settings")
	TArray<FMapPlayerOperationArgs> OperationArgsArray;
};

UCLASS()
class P20260324_API UProjectMappableKeySetting : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame)
	TArray<FTaggedMapPlayerOperationArgs> TaggedOperationArgsArray;
};
