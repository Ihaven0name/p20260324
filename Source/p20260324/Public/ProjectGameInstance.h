#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"

class UInputInfo;

UCLASS()
class P20260324_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Info")
	TObjectPtr<UInputInfo> InputInfo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float MouseAxisXSensitivityScale=0.5f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float MouseAxisYSensitivityScale=0.1f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float MouseMiddleSensitivityScale=1.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mouse")
	float SpringArmChangeSensitivityScale=10.f;
	
};
