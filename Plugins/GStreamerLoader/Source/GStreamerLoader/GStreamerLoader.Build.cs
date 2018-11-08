using UnrealBuildTool;

public class GStreamerLoader : ModuleRules
{
    public GStreamerLoader(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );
    }
}
