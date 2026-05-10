// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UnLuaInterface.h"
#include "LuaWorldSubsystem.generated.h"

class ULevelStreamingDynamic;
/**
 * 
 */
UCLASS()
class LUAFRAMEWORK_API ULuaWorldSubsystem : public UWorldSubsystem,public IUnLuaInterface
{
	GENERATED_BODY()
public:
	virtual FString GetModuleName_Implementation()const override
	{
		return TEXT("Plugins.LuaFrameWork.Service.WorldService");
	}
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	//生命周期的四个阶段
	UFUNCTION(BlueprintImplementableEvent)
	void LuaInit();
	UFUNCTION(BlueprintImplementableEvent)
	void LuaPostInit();
	UFUNCTION(BlueprintImplementableEvent)
	void LuaOnWorldBeginPlay(UWorld* World);
	UFUNCTION(BlueprintImplementableEvent)
	void LuaDeinit();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetWidgetByPath(FString Path);
	// UFUNCTION(BlueprintCallable)
	// ULevelStreamingDynamic* LoadLevelByPath(FString Path,FVector Pos,FRotator Rotator);
};
