#include "Manager/TeamManager.h"

#include "Manager/ConfigManager.h"
#include "p20260324/LogChannel.h"

void UTeamManager::GetTeamInfoArray(TArray<FTeamCharacterInfo>& OutTeamArray)
{
	OutTeamArray.Empty(); // 先清空

	if (const APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		if (const AProjectPlayerState* PlayerState = PlayerController->GetPlayerState<AProjectPlayerState>())
		{
			OutTeamArray = PlayerState->TeamArray;
		}
	}
}

AProjectPlayerCharacter* UTeamManager::AddTeamInfo(const FGameplayTag CharacterTag, const FVector Location, const FRotator Rotation) const
{
	if (const APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		if (!PlayerController->HasAuthority()) return nullptr;
		if (AProjectPlayerState* PlayerState = PlayerController->GetPlayerState<AProjectPlayerState>())
		{
			return PlayerState->AddTeamInfo(CharacterTag, Location, Rotation);
		}
	}
	return nullptr;
}

AProjectPlayerCharacter* UTeamManager::GetControlledCharacter() const
{
	if (const APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		if (AProjectPlayerState* PlayerState = PlayerController->GetPlayerState<AProjectPlayerState>())
		{
			return PlayerState->TeamArray[PlayerState->ControlledCharacterIndex].Character;
		}
	}
	UE_LOG(LogProject,Warning,TEXT("UTeamManager::GetControlledCharacter Is Failed"));
	return nullptr;
}

int32 UTeamManager::GetControlledCharacterIndex() const
{
	if (const APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		if (AProjectPlayerState* PlayerState = PlayerController->GetPlayerState<AProjectPlayerState>())
		{
			return PlayerState->ControlledCharacterIndex;
		}
	}
	UE_LOG(LogProject,Warning,TEXT("UTeamManager::GetControlledCharacterIndex Is Failed"));
	return 0;
}

void UTeamManager::SetControlledCharacterIndex(const int32 Index) const
{
	if (const APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		if (!PlayerController->HasAuthority()) return;
		if (AProjectPlayerState* PlayerState = PlayerController->GetPlayerState<AProjectPlayerState>())
		{
			PlayerState->ControlledCharacterIndex = Index;
		}
	}
}
