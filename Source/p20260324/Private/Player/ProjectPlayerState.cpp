#include "Player/ProjectPlayerState.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/ConfigManager.h"
#include "Net/UnrealNetwork.h"
#include "Player/ProjectPlayerCharacter.h"
#include "Net/Core/PushModel/PushModel.h"
class UCharacterInfo;

AProjectPlayerState::AProjectPlayerState()
{
	
}


void AProjectPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 同步数组给所有客户端
	
	DOREPLIFETIME(AProjectPlayerState, ControlledCharacterIndex);
	DOREPLIFETIME(AProjectPlayerState, TeamArray);
}

AProjectPlayerCharacter* AProjectPlayerState::AddTeamInfo(FGameplayTag CharacterTag, FVector Location, FRotator Rotation)
{
	if (!HasAuthority()) return nullptr;
	if (TeamArray.Num() >= TeamBound) return nullptr;
	for (FTeamCharacterInfo& TempTeamCharacterInfo : TeamArray)
	{
		if (TempTeamCharacterInfo.CharacterTag.MatchesTagExact(CharacterTag)) return nullptr;
	}
	UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(this);
	UCharacterInfo* CharacterInfo = ConfigManager->GetCharacterInfo();
	UTeamInfo* TeamInfo = ConfigManager->GetTeamInfo();
	TSubclassOf<AProjectPlayerCharacter> CharacterClass;
	if (!CharacterInfo->FindCharacterClassByTag(CharacterTag,CharacterClass)) return nullptr;
	FActorSpawnParameters Params;
	Params.Owner = GetPlayerController();

	if (AProjectPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AProjectPlayerCharacter>(CharacterClass, Location, Rotation, Params))
	{
		NewCharacter->SetOwner(GetPlayerController());
		NewCharacter->SetPlayerState(this);
		NewCharacter->InitAbilityActorInfo();
		
		FTeamCharacterInfo NewInfo;
		NewInfo.CharacterTag = CharacterTag;
		NewInfo.Character = NewCharacter;
		NewInfo.AbilitySystemComponent = NewCharacter->GetAbilitySystemComponent();
		NewInfo.AttributeSet = NewCharacter->GetAttributeSet();
		TeamInfo->FindTeamStaticInfoByTag(CharacterTag,NewInfo.TeamStaticInfo);
        
		TeamArray.Add(NewInfo);
        
		// 7. 强制同步（如果是手动修改数组，有时需要调用这个）
		MARK_PROPERTY_DIRTY_FROM_NAME(AProjectPlayerState, TeamArray, this);
		return NewCharacter;
	}
	return nullptr;
}
