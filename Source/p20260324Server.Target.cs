 // Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class p20260324ServerTarget : TargetRules
{
	public p20260324ServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("p20260324");
	}
}
