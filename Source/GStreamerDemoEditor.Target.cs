using UnrealBuildTool;
using System.Collections.Generic;

public class GStreamerDemoEditorTarget : TargetRules
{
	public GStreamerDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		bUseUnityBuild = false;
		bUsePCHFiles = false;

		ExtraModuleNames.AddRange( new string[] { "GStreamerDemo" } );
	}
}
