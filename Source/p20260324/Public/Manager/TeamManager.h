#pragma once

#include "CoreMinimal.h"
#include "Player/ProjectPlayerState.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "TeamManager.generated.h"

UCLASS()
class P20260324_API UTeamManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Team")
	void GetTeamInfoArray(TArray<FTeamCharacterInfo>& OutTeamArray);
	UFUNCTION(BlueprintCallable)
	AProjectPlayerCharacter* AddTeamInfo(
		FGameplayTag CharacterTag,
		FVector Location = FVector::ZeroVector,
		FRotator Rotation = FRotator::ZeroRotator) const;
	UFUNCTION(BlueprintCallable)
	AProjectPlayerCharacter* GetControlledCharacter() const;
	UFUNCTION(BlueprintCallable)
	int32 GetControlledCharacterIndex() const;
	UFUNCTION(BlueprintCallable)
	void SetControlledCharacterIndex(int32 Index) const;
};
