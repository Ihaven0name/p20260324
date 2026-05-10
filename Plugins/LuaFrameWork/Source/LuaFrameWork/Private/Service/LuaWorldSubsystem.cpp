// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/LuaWorldSubsystem.h"

#include "Blueprint/UserWidget.h"

bool ULuaWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void ULuaWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LuaInit();
}

void ULuaWorldSubsystem::PostInitialize()
{
	LuaPostInit();
}

void ULuaWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	LuaOnWorldBeginPlay(&InWorld);
}

void ULuaWorldSubsystem::Deinitialize()
{
	if (IsValid(this))
		LuaDeinit();
}

UUserWidget* ULuaWorldSubsystem::GetWidgetByPath(FString Path)
{
	if (UClass* WidgetClass = LoadObject<UClass>(nullptr,*Path))
	{
		UUserWidget* Widget = NewObject<UUserWidget>(GetWorld(),WidgetClass);
		return Widget;
	}
	return nullptr;
}

// ULevelStreamingDynamic* ULuaWorldSubsystem::LoadLevelByPath(FString Path, FVector Pos, FRotator Rotator)
// {
// }
