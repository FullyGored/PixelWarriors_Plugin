// Copyright Pixel Warriors Inc. 2025
// PixelWarriorsRuntime.Build.cs (in Plugins/GameFeatures/PixelWarriors/Source/PixelWarriorsRuntime/)
using UnrealBuildTool;

public class PixelWarriorsRuntime : ModuleRules
{
	public PixelWarriorsRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"ModularGameplay",
				"CommonGame",
				"LyraGame" // <-- THE KILL-SHOT. IT IS, AND ALWAYS WAS, A PUBLIC DEPENDENCY.
			}
		);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"GameplayAbilities",
				"GameplayTasks",
				"CommonUI",
				"UMG",
				"Slate",
				"SlateCore",
				"AIModule"
			}
		);
	}
}