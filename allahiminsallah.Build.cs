// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class allahiminsallah : ModuleRules
{

	private string ThirdPartyLib
    {
		get { return "C:/Program Files/Java/jdk-11.0.15"; }
    }

	public bool LoadJni(ReadOnlyTargetRules Rules)
    {
		bool isLibrarySupported = false;
		string jniPath = ThirdPartyLib;
		string jniLibPath = "";
		bool isDebug = Target.Configuration == UnrealTargetConfiguration.Debug;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			jniLibPath = Path.Combine(jniPath, "bin", "server");
			isLibrarySupported = true;
		}
		if (isLibrarySupported)
        {
			PublicIncludePaths.AddRange(new string[] { 
				Path.Combine(jniPath, "include"),
				Path.Combine(jniPath, "include", "win32"),
				Path.Combine(jniPath, "include", "win32", "bridge")
			});
			PublicDelayLoadDLLs.Add(Path.Combine(jniLibPath, "jvm.dll"));
		}
		return isLibrarySupported;
    }

	public allahiminsallah(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {  "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		bEnableUndefinedIdentifierWarnings = false;

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		LoadJni(Target);
	}
}
