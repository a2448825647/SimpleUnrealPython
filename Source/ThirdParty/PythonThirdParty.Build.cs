// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class PythonThirdParty : ModuleRules
{
    private void CopyDllAndLibToPluginBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string BinariesDirectory = Path.Combine(ModuleDirectory, "../../../../Binaries", Target.Platform.ToString());
        //string BinariesDirectory = Path.Combine(ModuleDirectory, "../../Binaries", Target.Platform.ToString());

        string Filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(BinariesDirectory))
        {
            Directory.CreateDirectory(BinariesDirectory);
        }

        if (!File.Exists(Path.Combine(BinariesDirectory, Filename)))
        {
            File.Copy(Filepath, Path.Combine(BinariesDirectory, Filename), true);
        }

        PublicDelayLoadDLLs.Add(Path.Combine(BinariesDirectory, Filename));
        RuntimeDependencies.Add(Path.Combine(BinariesDirectory, Filename));
    }


    public PythonThirdParty(ReadOnlyTargetRules Target) : base(Target)
	{
        Type = ModuleType.External;

        RuntimeDependencies.Add("$(ModuleDir)/Python/...");
        RuntimeDependencies.Add("$(ModuleDir)/PythonPackages/...");
        RuntimeDependencies.Add("$(ModuleDir)/Scripts/...");
        PublicIncludePaths.AddRange(
            new string[] {
                "$(ModuleDir)/Python/include",
                //"$(ModuleDir)/PythonPackages",
                //"$(ModuleDir)/Scripts",
				// ... add public include paths required here ...
			}
            );
        PublicAdditionalLibraries.AddRange(
            new string[] {
                //"$(PluginDir)/Source/ThirdParty/Python/libs/_tkinter.lib",
                //"$(PluginDir)/Source/ThirdParty/Python/libs/python3.lib",
                "$(PluginDir)/Source/ThirdParty/Python/libs/python39.lib",
            }
            );
        ////采用提供python39安装程序方式
        string DllPath = Path.Combine(ModuleDirectory, "Python/libs");
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "vcruntime140.dll"), Target);
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "vcruntime140_1.dll"), Target);
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "libcrypto-1_1.dll"), Target);
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "libffi-7.dll"), Target);
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "libssl-1_1.dll"), Target);
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "sqlite3.dll"), Target);
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "python3.dll"), Target);
        CopyDllAndLibToPluginBinaries(Path.Combine(DllPath, "python39.dll"), Target);
        string BatPath = Path.Combine(ModuleDirectory, "Python", "检测运行环境.bat");
        string CopyToPath = Path.Combine(ModuleDirectory, "../../../../", "检测运行环境.bat");
        if (!File.Exists(CopyToPath))
        {
            File.Copy(BatPath, CopyToPath, true);
        }
        RuntimeDependencies.Add(CopyToPath);
    }
}
