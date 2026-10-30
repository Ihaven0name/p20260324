// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class P20260324_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** 执行对象自身的交互逻辑。 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="交互")
	void Interact(AActor* InteractingActor);
};
