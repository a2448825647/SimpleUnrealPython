// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PythonVM : ModuleRules
{
	public PythonVM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"$(ModuleDir)/../ThirdParty/Python/include",
				"$(ModuleDir)/../ThirdParty/PythonPackages"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {

				// ... add other private include paths required here ...
			}
			);

		PublicLibraryPaths.AddRange(
			new string[] {
				"$(ModuleDir)/../ThirdParty/Python/libs"
			}
			);
		PublicAdditionalLibraries.AddRange(
			new string[] {
				"python39.lib"
			}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"PythonVMLibrary",
				"Projects"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
