
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ProjectAssetManager.generated.h"


UCLASS()
class P20260324_API UProjectAssetManager : public UAssetManager
{
	GENERATED_BODY()
	virtual void StartInitialLoading() override;
	
	static UProjectAssetManager& Get();
};
