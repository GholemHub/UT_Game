// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UT_Game : ModuleRules
{
	public UT_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
