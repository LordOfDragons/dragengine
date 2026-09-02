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

$LibVersion = "1.5.0"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libwebp-$LibVersion.tar.xz" -UrlPath "libwebp"

Expand-TarXz -Path "$ProjectDir\libwebp-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = "$ExpandedDir\libwebp-$LibVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DBUILD_SHARED_LIBS=Off `
	-DWEBP_BUILD_ANIM_UTILS=Off `
	-DWEBP_BUILD_CWEBP=Off `
	-DWEBP_BUILD_DWEBP=Off `
	-DWEBP_BUILD_GIF2WEBP=Off `
	-DWEBP_BUILD_IMG2WEBP=Off `
	-DWEBP_BUILD_INFO=Off `
	-DWEBP_BUILD_MUXTOOLS=Off `
	-DWEBP_BUILD_VWEBP=Off `
	-DWEBP_BUILD_WEBPINFO=Off `
	-DWEBP_BUILD_EXTRAS=Off `
	-DCMAKE_SYSTEM_NAME=Windows `
	-DCMAKE_SYSTEM_PROCESSOR=AMD64 `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5"

cmake --build "$CmakeBuildDir" -j 8 -- /property:Configuration=Release

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}

Copy-Item "$CmakeBuildDir\cmake_install.cmake" -Destination $CmakeInstallDir -Force

cmake --install "$CmakeInstallDir"

Remove-Item "$CmakeInstallDir\cmake_install.cmake" -Force
