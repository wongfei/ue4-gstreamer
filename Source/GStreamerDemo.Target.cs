using UnrealBuildTool;
using System.Collections.Generic;

public class GStreamerDemoTarget : TargetRules
{
	public GStreamerDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		bUseUnityBuild = false;
		bUsePCHFiles = false;

		ExtraModuleNames.AddRange( new string[] { "GStreamerDemo" } );
	}
}
