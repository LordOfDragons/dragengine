param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$LibraryVersion = "1.5.2"
$ExpandedDir = "$ProjectDir\build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "dragonscript-$LibraryVersion.tar.xz" -UrlPath "dragonscript"

Expand-TarXz -Path "$ProjectDir\dragonscript-$LibraryVersion.tar.xz" -Destination $ExpandedDir

# Build the downloaded Visual Studio solution in-place.
# Locate MSBuild via vswhere if available, otherwise fall back to msbuild.exe in PATH.
$ExpandedSrcDir = "$ExpandedDir\dragonscript-$LibraryVersion"
$slnPath = "$ExpandedSrcDir\vs\dragonscript.sln"
$msbuild = Get-MSBuildPath
$dragengineToolset = Get-VisualStudioToolset
$platformVersion = Get-VisualStudioPlatformVersion
$configuration = "Release"
$platform = "x64"
$runtimeLibrary = "MultiThreadedDLL"

Get-ChildItem -Path "$ExpandedSrcDir\vs" -Filter *.vcxproj -Recurse | ForEach-Object {
    (Get-Content $_.FullName) `
        -replace '<PlatformToolset>.*</PlatformToolset>', "<PlatformToolset>$dragengineToolset</PlatformToolset>" `
        -replace '<WindowsTargetPlatformVersion>.*</WindowsTargetPlatformVersion>', "<WindowsTargetPlatformVersion>$platformVersion</WindowsTargetPlatformVersion>" `
        -replace '<RuntimeLibrary>.*</RuntimeLibrary>', "<RuntimeLibrary>$runtimeLibrary</RuntimeLibrary>" `
        | Set-Content $_.FullName
}

Write-Host "Building solution: $slnPath"
Write-Host "Using MSBuild: $msbuild"

& $msbuild $slnPath /m /t:Build /p:Configuration=$configuration /p:Platform=$platform `
	/p:PlatformToolset="$dragengineToolset" /p:RuntimeLibrary="$runtimeLibrary" `
    /p:WindowsTargetPlatformVersion="$platformVersion"
if ($LASTEXITCODE -ne 0) {
	Write-Error "MSBuild failed with exit code $LASTEXITCODE"
	throw "MSBuild failed with exit code $LASTEXITCODE"
}

# copy the built static library to the expected location
$projectDir = "$ExpandedSrcDir\vs"
$libBuildDir = "$projectDir\x64\Release"
$targetLibDir = "$ExpandedDir\install\lib"

if (-not (Test-Path $targetLibDir)) {
	New-Item -Path $targetLibDir -ItemType Directory | Out-Null
}

Copy-Item "$libBuildDir\libdscript-static.lib" -Destination $targetLibDir -Force
Copy-Item "$libBuildDir\libdscript-static.pdb" -Destination $targetLibDir -Force

# copy the include directory
$sourceIncludeDir = "$libBuildDir\Distribute\SDK\include"
$targetIncludeDir = "$ExpandedDir\install\include"

if (Test-Path $targetIncludeDir) {
	Remove-Item $targetIncludeDir -Force -Recurse
}
Copy-Item $sourceIncludeDir -Destination $targetIncludeDir -Recurse -Force

# copy dsinstall directory
$sourceDsinstallDir = "$libBuildDir\Distribute\SDK\dsinstall"
$targetDsinstallDir = "$ExpandedDir\install\dsinstall"

if (Test-Path $targetDsinstallDir) {
	Remove-Item $targetDsinstallDir -Force -Recurse
}
Copy-Item $sourceDsinstallDir -Destination $targetDsinstallDir -Recurse -Force
