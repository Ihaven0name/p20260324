// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SceneActorInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class USceneActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class P20260324_API ISceneActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** 接收射线命中结果并播放对应的受击表现。 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Scene Actor|Hit")
	void OnHitReceived(const TArray<FHitResult>& HitResults, AActor* HitInstigator);
};
