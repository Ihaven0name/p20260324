#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TeamInfo.generated.h"


USTRUCT(BlueprintType)
struct FTeamStaticInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> CharacterTexture;
};


UCLASS()
class P20260324_API UTeamInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FTeamStaticInfo> CharacterTagToTeamStaticInfo;
	
	UFUNCTION(BlueprintCallable, Category = "Team")
	bool FindTeamStaticInfoByTag(const FGameplayTag& CharacterTag, FTeamStaticInfo& OutTeamStaticInfo) const;
};
