
#include "GameMode/PlayGameModeBase.h"

#include "GAS/ProjectGameplayTag.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ProjectPlayerCharacter.h"
#include "Player/ProjectPlayerController.h"
#include "Player/ProjectPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		AProjectPlayerState* PlayerState = NewPlayer
			? NewPlayer->GetPlayerState<AProjectPlayerState>()
			: nullptr;
		if (!PlayerState)
		{
			UE_LOG(LogTemp, Error, TEXT("PostLogin: 无法获取新玩家的 ProjectPlayerState，未能创建初始角色。"));
			return;
		}

		AProjectPlayerCharacter* PlayerCharacter = PlayerState->AddTeamInfo(
			FProjectGameplayTag::Get().Character_Crouch,
			DefaultCharacterLocation,
			DefaultCharacterRotation);
		if (!IsValid(PlayerCharacter))
		{
			UE_LOG(LogTemp, Error, TEXT("PostLogin: 创建新玩家初始角色失败。"));
			return;
		}

		NewPlayer->Possess(PlayerCharacter);
	}
}

bool APlayGameModeBase::RespawnDeadPlayer(APlayerController* PlayerController)
{
	if (!HasAuthority() || !IsValid(PlayerController) || !GetWorld())
	{
		return false;
	}

	AProjectPlayerCharacter* DeadCharacter = Cast<AProjectPlayerCharacter>(PlayerController->GetPawn());
	AProjectPlayerState* ProjectPlayerState = PlayerController->GetPlayerState<AProjectPlayerState>();
	if (!IsValid(DeadCharacter) || !DeadCharacter->bIsDead || !IsValid(ProjectPlayerState))
	{
		return false;
	}

	FVector SpawnLocation = DeadCharacter->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);
	FRotator SpawnRotation = DeadCharacter->GetActorRotation();
	if (AActor* PlayerStart = FindPlayerStart(PlayerController))
	{
		SpawnLocation = PlayerStart->GetActorLocation();
		SpawnRotation = PlayerStart->GetActorRotation();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AProjectPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AProjectPlayerCharacter>(
		DeadCharacter->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
	if (!IsValid(NewCharacter))
	{
		return false;
	}

	NewCharacter->SetOwner(PlayerController);
	NewCharacter->SetPlayerState(ProjectPlayerState);
	NewCharacter->InitAbilityActorInfo();
	if (!ProjectPlayerState->ReplaceTeamCharacter(DeadCharacter, NewCharacter))
	{
		NewCharacter->Destroy();
		return false;
	}

	if (UProjectAbilitySystemComponent* DeadASC = DeadCharacter->GetAbilitySystemComponent())
	{
		DeadASC->CancelAllAbilities();
		DeadASC->SetUserAbilityActivationInhibited(true);
	}

	if (UCharacterMovementComponent* Movement = DeadCharacter->GetCharacterMovement())
	{
		Movement->StopMovementImmediately();
		Movement->DisableMovement();
	}
	PlayerController->Possess(NewCharacter);
	if (PlayerController->GetPawn() != NewCharacter)
	{
		ProjectPlayerState->ReplaceTeamCharacter(NewCharacter, DeadCharacter);
		NewCharacter->Destroy();
		return false;
	}

	DeadCharacter->Destroy();
	return true;
}
