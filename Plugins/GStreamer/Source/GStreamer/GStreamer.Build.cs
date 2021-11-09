using UnrealBuildTool;
using System.IO;

public class GStreamer : ModuleRules
{
    public GStreamer(ReadOnlyTargetRules Target) : base(Target)
    {
        DefaultBuildSettings = BuildSettingsVersion.V2;
        PCHUsage = PCHUsageMode.NoPCHs; // UseExplicitOrSharedPCHs;
        bUseUnity = false;
        bEnableUndefinedIdentifierWarnings = false;

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "RenderCore",
                "Slate",
                "SlateCore"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            const string GStreamerRoot = @"C:\dev\gstreamer_dev\1.0\msvc_x86_64"; // path to gstreamer development package

            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "include"));
            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "include", "gstreamer-1.0"));
            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "include", "glib-2.0"));
            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "lib", "glib-2.0", "include"));

            var GStreamerLibPath = Path.Combine(GStreamerRoot, "lib");
            PublicSystemLibraryPaths.Add(GStreamerLibPath);

            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "glib-2.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gobject-2.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gstreamer-1.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gstvideo-1.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gstapp-1.0.lib"));

            PublicDelayLoadDLLs.Add("glib-2.0-0.dll");
            PublicDelayLoadDLLs.Add("gobject-2.0-0.dll");
            PublicDelayLoadDLLs.Add("gstreamer-1.0-0.dll");
            PublicDelayLoadDLLs.Add("gstvideo-1.0-0.dll");
            PublicDelayLoadDLLs.Add("gstapp-1.0-0.dll");
        }
    }
}
