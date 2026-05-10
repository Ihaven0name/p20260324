// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectThreadSafeFunctionLibrary.generated.h"

UCLASS(meta=(BlueprintThreadSafe))
class P20260324_API UProjectThreadSafeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetAngleFromVectors(FVector ReferenceDir, FVector TargetDir);
};
