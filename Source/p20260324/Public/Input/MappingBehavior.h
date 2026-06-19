// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "MappingBehavior.generated.h"

class UInputModifier;
class UInputTrigger;
class UInputAction;
//一种操作对应两种输入键，分为主次键
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class P20260324_API UMappingBehavior : public UObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BehaviorName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag BehaviorTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag IMCTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UInputTrigger>> InputTriggers;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UInputModifier>> InputModifiers;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText BehaviorDisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Support Secondary Slot"))
	bool bSupportSecondarySlot = true;
};
