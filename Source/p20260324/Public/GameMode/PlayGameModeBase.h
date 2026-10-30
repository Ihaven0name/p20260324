
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayGameModeBase.generated.h"


UCLASS()
class P20260324_API APlayGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** 仅为请求的死亡玩家生成新 Pawn；不会重载整场关卡。 */
	bool RespawnDeadPlayer(APlayerController* PlayerController);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Default Character")
	FVector DefaultCharacterLocation = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Default Character")
	FRotator DefaultCharacterRotation = FRotator::ZeroRotator;
};
