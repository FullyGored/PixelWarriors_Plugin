// Copyright Pixel Warriors Inc. 2025
// PixelWarriorsRuntime.Build.cs (in Plugins/GameFeatures/PixelWarriors/Source/PixelWarriorsRuntime/)

using System.IO;
using UnrealBuildTool;

public class PixelWarriorsRuntime : ModuleRules
{
	public PixelWarriorsRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"GameplayTags",
				"GameplayTasks",
				"CommonUI",
				"ModularGameplay",
				"ModularGameplayActors",  // ← FIXES AModularGameStateBase unresolved symbols
				"LyraGame"               // ← FIXES ALyraGameMode unresolved symbols
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AIModule"  // For GenericTeamAgentInterface
			}
		);

		// Optional: If Intellisense/linker still fails, add explicit include paths to LyraGame
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../../Source/LyraGame"));
	}
}