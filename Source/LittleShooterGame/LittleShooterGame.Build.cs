// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LittleShooterGame : ModuleRules
{
	public LittleShooterGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

		PublicIncludePaths.AddRange(new string[] {
			"LittleShooterGame/",
			"LittleShooterGame/Public/Weapon"
		});

	}
}
