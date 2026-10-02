// Copyright Ian Good

using UnrealBuildTool;

public class IGShowme : ModuleRules
{
	public IGShowme(ReadOnlyTargetRules Target) : base(Target)
	{
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Error;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
		});
	}
}
