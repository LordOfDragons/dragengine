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

$LibVersion = "1.3.2"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libogg-$LibVersion.tar.xz" -UrlPath "libogg"

Expand-TarXz -Path "$ProjectDir\libogg-$LibVersion.tar.xz" -Destination $ExpandedDir

# build static library using Visual Studio solution file
$ExpandedSrcDir = "$ExpandedDir\libogg-$LibVersion"
$vcxprojPath = "$ExpandedSrcDir\win32\VS2010\libogg_static.vcxproj"
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

Write-Host "Building project: $vcxprojPath"
Write-Host "Using MSBuild: $msbuild"
& $msbuild $vcxprojPath /m /t:Build /p:Configuration=$configuration /p:Platform=$platform `
	/p:PlatformToolset="$dragengineToolset" /p:RuntimeLibrary="$runtimeLibrary" `
    /p:WindowsTargetPlatformVersion="$platformVersion" `
    /p:ClCommandLineParameters="/MD" /p:LinkIncremental=No
if ($LASTEXITCODE -ne 0) {
	Write-Error "MSBuild failed with exit code $LASTEXITCODE"
	throw "MSBuild failed with exit code $LASTEXITCODE"
}

# copy the built static library to the expected location
$projectDir = Split-Path -Path $vcxprojPath -Parent
$libBuildDir = "$projectDir\x64\Release"
$targetLibDir = "$ExpandedDir\install\lib"

if (-not (Test-Path $targetLibDir)) {
	New-Item -Path $targetLibDir -ItemType Directory | Out-Null
}
Copy-Item "$libBuildDir\libogg_static.lib" -Destination "$targetLibDir\libogg_static.lib" -Force

# copy the include directory
$sourceIncludeDir = "$ExpandedSrcDir\include"
$targetIncludeDir = "$ExpandedDir\install\include"

if (Test-Path $targetIncludeDir) {
	Remove-Item $targetIncludeDir -Force -Recurse
}
Copy-Item $sourceIncludeDir -Destination $targetIncludeDir -Recurse -Force
