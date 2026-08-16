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

$LibVersion = "2.1.0"

$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
	Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "svt-av1-$LibVersion.tar.xz" -UrlPath "svtav1"

Expand-TarXz -Path "$ProjectDir\svt-av1-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = Join-Path -Path $ExpandedDir -ChildPath "svt-av1-$LibVersion"
$CmakeBuildDir = Join-Path -Path $ExpandedDir -ChildPath "build"
$CmakeInstallDir = Join-Path -Path $ExpandedDir -ChildPath "install"

if (!(Test-Path $CmakeBuildDir)) {
	New-Item -Path $CmakeBuildDir -ItemType "directory" -Force | Out-Null
}

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DBUILD_SHARED_LIBS=Off `
	-DCMAKE_SYSTEM_NAME=Windows `
	-DCMAKE_SYSTEM_PROCESSOR=AMD64 `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5" `
	-DBUILD_TESTING=OFF `
	-DBUILD_APPS=OFF `
	-DBUILD_DEC=OFF

cmake --build "$CmakeBuildDir" --config Release -j 8

if (!(Test-Path $CmakeInstallDir)) {
	New-Item -Path $CmakeInstallDir -ItemType "directory" -Force | Out-Null
}

cmake --install "$CmakeBuildDir" --config Release --prefix "$CmakeInstallDir"
