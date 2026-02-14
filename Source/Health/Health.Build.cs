// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Health : ModuleRules
{
    public Health(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",           // UI À§Á¬¿ë
            "Slate",         // UI¿ë
            "SlateCore"      // UI¿ë
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}