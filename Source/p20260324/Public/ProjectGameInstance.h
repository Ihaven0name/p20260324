#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "ProjectGameInstance.generated.h"

class UAbilityInfo;
class UCharacterInfo;

UCLASS()
class P20260324_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float MouseAxisXSensitivityScale=0.5f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float MouseAxisYSensitivityScale=0.1f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float MouseMiddleSensitivityScale=1.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float SpringArmChangeSensitivityScale=10.f;
	
	// 锁定切换灵敏度
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LockOn")
	float LockOnSwitchSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="XP")
	TMap<FGameplayTag, int32> XPTagToXPValue;
	
};
