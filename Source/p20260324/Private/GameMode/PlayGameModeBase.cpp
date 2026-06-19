
#include "GameMode/PlayGameModeBase.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Info/TeamInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/TeamManager.h"

class AProjectPlayerState;

void APlayGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AutoSave"), 0))
	{
		// // 读档
		// UProjectSaveGame* LoadObj = Cast<UProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"), 0));
		// TeamToSpawn = LoadObj->SavedTeamTags;
	}
	else
	{
		UTeamManager* TeamManager=UProjectBlueprintFunctionLibrary::GetTeamManager(NewPlayer);
		AProjectPlayerCharacter* TempPlayerCharacter = TeamManager->AddTeamInfo(
			FProjectGameplayTag::Get().Character_Crouch,
			DefaultCharacterLocation,
			DefaultCharacterRotation
		);
		NewPlayer->Possess(TempPlayerCharacter);
	}
}
