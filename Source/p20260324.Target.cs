
using UnrealBuildTool;
using System.Collections.Generic;

public class p20260324Target : TargetRules
{
	public p20260324Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("p20260324");
	}
}
