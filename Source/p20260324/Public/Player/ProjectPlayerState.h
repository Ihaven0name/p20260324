#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "Info/TeamInfo.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamBound=3;

	UFUNCTION(BlueprintCallable)
	AProjectPlayerCharacter* AddTeamInfo(
		FGameplayTag CharacterTag,
		FVector Location = FVector::ZeroVector,
		FRotator Rotation = FRotator::ZeroRotator);
	
};
