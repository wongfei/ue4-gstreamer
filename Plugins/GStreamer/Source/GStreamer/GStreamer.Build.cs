using UnrealBuildTool;
using System.IO;

public class GStreamer : ModuleRules
{
    public GStreamer(ReadOnlyTargetRules Target) : base(Target)
    {
        const string GStreamerRoot = @"C:\dev\gstreamer_dev\1.0\msvc_x86_64";

        DefaultBuildSettings = BuildSettingsVersion.V2;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
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
                "SlateCore",
                "GStreamerLoader"
            }
        );

        PublicIncludePaths.Add(Path.Combine(GStreamerRoot, "include"));
        PublicIncludePaths.Add(Path.Combine(GStreamerRoot, "include", "gstreamer-1.0"));
        PublicIncludePaths.Add(Path.Combine(GStreamerRoot, "include", "glib-2.0"));
        PublicIncludePaths.Add(Path.Combine(GStreamerRoot, "lib", "glib-2.0", "include"));
        PublicLibraryPaths.Add(Path.Combine(GStreamerRoot, "lib"));
        PublicAdditionalLibraries.Add("glib-2.0.lib");
        PublicAdditionalLibraries.Add("gobject-2.0.lib");
        PublicAdditionalLibraries.Add("gstreamer-1.0.lib");
        PublicAdditionalLibraries.Add("gstvideo-1.0.lib");
    }
}
