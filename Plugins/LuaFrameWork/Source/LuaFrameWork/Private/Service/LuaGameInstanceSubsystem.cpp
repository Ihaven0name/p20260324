// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/LuaGameInstanceSubsystem.h"

bool ULuaGameInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void ULuaGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LuaInit();
}

void ULuaGameInstanceSubsystem::Deinitialize()
{
	
	LuaDeinit();
}
