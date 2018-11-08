using UnrealBuildTool;
using System.Collections.Generic;

public class GStreamerDemoTarget : TargetRules
{
	public GStreamerDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		bUseUnityBuild = false;
		bUsePCHFiles = false;

		ExtraModuleNames.AddRange( new string[] { "GStreamerDemo" } );
	}
}
