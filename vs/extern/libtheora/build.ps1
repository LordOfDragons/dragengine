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

$LibVersion = "1.1.1"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "theora-$LibVersion.tar.xz" -UrlPath "libtheora"

Expand-TarXz -Path "$ProjectDir\theora-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = "$ExpandedDir\theora-$LibVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

$OggIncludeDir = Join-Path -Path $ProjectDir -ChildPath "..\libogg\build\install\include"
$OggLibDir = Join-Path -Path $ProjectDir -ChildPath "..\libogg\build\install\lib"
$VorbisIncludeDir = Join-Path -Path $ProjectDir -ChildPath "..\libvorbis\build\install\include"
$VorbisLibDir = Join-Path -Path $ProjectDir -ChildPath "..\libvorbis\build\install\lib"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DBUILD_SHARED_LIBS=Off `
	-DCMAKE_SYSTEM_NAME=Windows `
	-DCMAKE_SYSTEM_PROCESSOR=AMD64 `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5" `
	-DOgg_INCLUDE_DIR="$OggIncludeDir" `
	-DOgg_LIBRARY_RELEASE="$OggLibDir\ogg.lib" `
	-DVorbis_INCLUDE_DIR="$VorbisIncludeDir" `
	-DVorbis_LIBRARY_RELEASE="$VorbisLibDir\vorbis.lib"

cmake --build "$CmakeBuildDir" -j 8 -- /property:Configuration=Release

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}

Copy-Item "$CmakeBuildDir\cmake_install.cmake" -Destination $CmakeInstallDir -Force

cmake --install "$CmakeInstallDir"

Remove-Item "$CmakeInstallDir\cmake_install.cmake" -Force
