#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "Info/TeamInfo.h"
#include "Manager/InventoryManager.h"
#include "ProjectPlayerState.generated.h"


class UProjectAbilitySystemComponent;
class UProjectAttributeSet;
class UAbilitySystemComponent;
class AProjectPlayerCharacter;

USTRUCT(BlueprintType)
struct FTeamCharacterInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag CharacterTag;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AProjectPlayerCharacter> Character;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UProjectAttributeSet> AttributeSet;
	UPROPERTY(BlueprintReadOnly)
	FTeamStaticInfo TeamStaticInfo;
};


UCLASS()
class P20260324_API AProjectPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AProjectPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	int32 ControlledCharacterIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	TArray<FTeamCharacterInfo> TeamArray;
	/** Server-owned inventory; replicated only to this player's connection. */
	UPROPERTY(ReplicatedUsing=OnRep_InventoryItems, BlueprintReadOnly, Category="Inventory")
	TArray<FInventoryItem> InventoryItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamBound=3;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetInventoryItemQuantity(FGameplayTag ItemTag) const;
	/** Server-side item grant entry point. */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddInventoryItem(FGameplayTag ItemTag, int32 Count);
	/** Atomically verifies and consumes the three XP material counts. */
	bool ConsumeExperienceMaterials(int32 SmallCount, int32 MediumCount, int32 LargeCount);

	UFUNCTION(BlueprintCallable)
	AProjectPlayerCharacter* AddTeamInfo(
		FGameplayTag CharacterTag,
		FVector Location = FVector::ZeroVector,
		FRotator Rotation = FRotator::ZeroRotator);
	bool ReplaceTeamCharacter(AProjectPlayerCharacter* OldCharacter, AProjectPlayerCharacter* NewCharacter);

	UFUNCTION()
	void OnRep_InventoryItems();
};
