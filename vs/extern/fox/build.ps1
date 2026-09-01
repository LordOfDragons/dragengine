param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$LibVersion = "1.7.85"
$DllVersion = "1.7"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "fox-$LibVersion.tar.xz" -UrlPath "fox"

Expand-TarXz -Path "$ProjectDir\fox-$LibVersion.tar.xz" -Destination $ExpandedDir

# build static library using Visual Studio solution file
$ExpandedSrcDir = "$ExpandedDir\fox-$LibVersion"
$solutionPath = "$ExpandedSrcDir\windows\vs.sln"
$msbuild = Get-MSBuildPath
$dragengineToolset = Get-VisualStudioToolset
$platformVersion = Get-VisualStudioPlatformVersion
$configuration = "Release"
$platform = "x64"
$runtimeLibrary = "MultiThreadedDLL"

Get-ChildItem -Path $ExpandedSrcDir -Filter *.vcxproj -Recurse | ForEach-Object {
    (Get-Content $_.FullName) `
        -replace '<PlatformToolset>.*</PlatformToolset>', "<PlatformToolset>$dragengineToolset</PlatformToolset>" `
        -replace '<WindowsTargetPlatformVersion>.*</WindowsTargetPlatformVersion>', "<WindowsTargetPlatformVersion>$platformVersion</WindowsTargetPlatformVersion>" `
        -replace '<RuntimeLibrary>.*</RuntimeLibrary>', "<RuntimeLibrary>$runtimeLibrary</RuntimeLibrary>" `
        | Set-Content $_.FullName
}

(Get-Content "$ExpandedSrcDir\windows\foxdll\foxdll.vcxproj") `
    -replace '<ConfigurationType>DynamicLibrary</ConfigurationType>', @"
        <ConfigurationType>DynamicLibrary</ConfigurationType>
        <TargetName>fox-$DllVersion</TargetName>
"@ `
    -replace ';HAVE_GLU_H;%\(', ';HAVE_GL_H;HAVE_PNG_H;HAVE_ZLIB_H;%(' `
    -replace '<PrecompiledHeaderFile />', @'
        <PrecompiledHeaderFile />
        <AdditionalIncludeDirectories>..\..\..\..\..\libpng\build\install\include;..\..\..\..\..\zlib\build\install\include</AdditionalIncludeDirectories>
'@ `
    -replace '<OptimizeReferences>true</OptimizeReferences>', @'
        <OptimizeReferences>true</OptimizeReferences>
        <AdditionalLibraryDirectories>..\..\..\..\..\libpng\build\install\lib;..\..\..\..\..\zlib\build\install\lib</AdditionalLibraryDirectories>
'@ `
    -replace ';glu32.lib;%\(', ';glu32.lib;libpng16_static.lib;zlibstatic.lib;%(' `
    | Set-Content "$ExpandedSrcDir\windows\foxdll\foxdll.vcxproj"

Write-Host "Building project: $solutionPath"
Write-Host "Using MSBuild: $msbuild"

& $msbuild $solutionPath /m /t:foxdll /p:Configuration=$configuration /p:Platform=$platform `
	/p:PlatformToolset="$dragengineToolset" /p:RuntimeLibrary="$runtimeLibrary" `
    /p:WindowsTargetPlatformVersion="$platformVersion"
if ($LASTEXITCODE -ne 0) {
	Write-Error "MSBuild failed with exit code $LASTEXITCODE"
	throw "MSBuild failed with exit code $LASTEXITCODE"
}

# copy libraries to build directory
$targetLibDir = "$ExpandedDir\install\lib"
$libBuildDir = "$ExpandedSrcDir\windows\x64\Release"

if (-not (Test-Path $targetLibDir)) {
	New-Item -Path $targetLibDir -ItemType Directory | Out-Null
}

Copy-Item -Path "$libBuildDir\fox-*.dll" -Destination "$targetLibDir" -Force
Copy-Item -Path "$libBuildDir\fox-*.lib" -Destination "$targetLibDir" -Force
Copy-Item -Path "$libBuildDir\fox-*.pdb" -Destination "$targetLibDir" -Force
Copy-Item -Path "$libBuildDir\fox-*.exp" -Destination "$targetLibDir" -Force

# copy the include directory
$sourceIncludeDir = "$ExpandedSrcDir\include"
$targetIncludeDir = "$ExpandedDir\install\include\fox-$DllVersion"

if (Test-Path $sourceIncludeDir) {
	if (Test-Path $targetIncludeDir) {
		Remove-Item $targetIncludeDir -Force -Recurse
	}
	Copy-Item $sourceIncludeDir -Destination $targetIncludeDir -Recurse -Force
} else {
	throw "Include directory not found at: $sourceIncludeDir"
}
