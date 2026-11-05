// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class p20260324 : ModuleRules
{
	public p20260324(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","GameplayTags","GameplayTasks","GameplayAbilities", "EnhancedInput","Lua","UnLua" ,"LuaFrameWork", "CommonUI","CommonInput","NetCore", "Slate", "SlateCore", "AIModule", "HTTP", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ApplicationCore" });

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "AssetRegistry", "UnrealEd" });
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// 注册账号走自定义 HTTP API；不依赖 OnlineSubsystem。

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
