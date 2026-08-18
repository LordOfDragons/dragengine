param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


if (Test-Path "$PSScriptRoot\..\..\github_cached_externals") {
    return
}


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$LibVersion = "2.1.1"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "soundtouch-$LibVersion.tar.xz" -UrlPath "soundtouch"

Expand-TarXz -Path "$ProjectDir\soundtouch-$LibVersion.tar.xz" -Destination $ExpandedDir

# build static library using Visual Studio solution file. the upstream solution file is broken
# and outdated so we have to patch some files
$ExpandedSrcDir = "$ExpandedDir\soundtouch-$LibVersion"
$vcxprojPath = "$ExpandedSrcDir\source\SoundTouch\SoundTouch.vcxproj"
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
    /p:WindowsTargetPlatformVersion="$platformVersion"
if ($LASTEXITCODE -ne 0) {
	Write-Error "MSBuild failed with exit code $LASTEXITCODE"
	throw "MSBuild failed with exit code $LASTEXITCODE"
}

# copy the built static library to the expected location
$projectDir = Split-Path -Path $vcxprojPath -Parent
$libBuildDir = "$projectDir\x64\Release"
$libFile = "$libBuildDir\SoundTouch_x64.lib"
$targetLibDir = "$ExpandedDir\install\lib"

if (-not (Test-Path $targetLibDir)) {
	New-Item -Path $targetLibDir -ItemType Directory | Out-Null
}
Copy-Item $libFile -Destination "$targetLibDir\SoundTouch.lib" -Force

# copy the include directory
$sourceIncludeDir = "$ExpandedSrcDir\include"
$targetIncludeDir = "$ExpandedDir\install\include\soundtouch"

if (Test-Path $targetIncludeDir) {
	Remove-Item $targetIncludeDir -Force -Recurse
}
Copy-Item $sourceIncludeDir -Destination $targetIncludeDir -Recurse -Force
