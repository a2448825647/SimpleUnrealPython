// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class PythonVM : ModuleRules
{
	public int CopyFolder2(string sourceFolder, string destFolder)
	{
		string folderName = System.IO.Path.GetFileName(sourceFolder);
		string destfolderdir = System.IO.Path.Combine(destFolder, folderName);
		string[] filenames = System.IO.Directory.GetFileSystemEntries(sourceFolder);
		foreach (string file in filenames)// 遍历所有的文件和目录
		{
			if (System.IO.Directory.Exists(file))
			{
				string currentdir = System.IO.Path.Combine(destfolderdir, System.IO.Path.GetFileName(file));
				if (!System.IO.Directory.Exists(currentdir))
				{
					System.IO.Directory.CreateDirectory(currentdir);
				}
				CopyFolder2(file, destfolderdir);
			}
			else
			{
				string srcfileName = System.IO.Path.Combine(destfolderdir, System.IO.Path.GetFileName(file));
				if (!System.IO.Directory.Exists(destfolderdir))
				{
					System.IO.Directory.CreateDirectory(destfolderdir);
				}
				System.IO.File.Copy(file, srcfileName,true);
			}
		}

		return 1;
	}
	public PythonVM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		//RuntimeDependencies.Add("$(ProjectDir)/Plugins/PythonVM/Binaries/Scripts/...");
		//RuntimeDependencies.Add("$(ProjectDir)/Plugins/PythonVM/Binaries/ThirdParty/...");
		PublicIncludePaths.AddRange(
			new string[] {
				//"$(ModuleDir)/../../Binaries/ThirdParty/Python/include",
				//"$(ModuleDir)/../../Binaries/ThirdParty/PythonPackages"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {

				// ... add other private include paths required here ...
			}
			);

		PublicAdditionalLibraries.AddRange(
			new string[] {
				//"$(ModuleDir)/../../Binaries/ThirdParty/Python/libs/python39.lib"
			}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Projects",
				"PythonThirdParty"
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


		string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/"));
		string ScriptPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../Scripts/"));
		string BinariesThirdPartyDirectory = Path.Combine(ModuleDirectory, "../../Binaries/ThirdParty/");
		string BinariesScriptDirectory = Path.Combine(ModuleDirectory, "../../Binaries/Scripts/");
		//CopyFolder2(ThirdPartyPath, BinariesThirdPartyDirectory);
		//CopyFolder2(ScriptPath, BinariesScriptDirectory);
	}
}
