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

$LibVersion = "1.6.40"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libpng-$LibVersion.tar.xz" -UrlPath "libpng"

Expand-TarXz -Path "$ProjectDir\libpng-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = "$ExpandedDir\libpng-$LibVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

$ZlibIncludeDir = Join-Path -Path $ProjectDir -ChildPath "..\zlib\build\install\include"
$ZlibLibDir = Join-Path -Path $ProjectDir -ChildPath "..\zlib\build\install\lib"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DBUILD_SHARED_LIBS=Off `
	-DPNG_SHARED=Off `
	-DPNG_STATIC=On `
	-DCMAKE_SYSTEM_NAME=Windows `
	-DCMAKE_SYSTEM_PROCESSOR=AMD64 `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5" `
	-DZLIB_INCLUDE_DIR="$ZlibIncludeDir" `
	-DZLIB_LIBRARY_RELEASE="$ZlibLibDir\zlibstatic.lib"

cmake --build "$CmakeBuildDir" -j 8 -- /property:Configuration=Release

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}

Copy-Item "$CmakeBuildDir\cmake_install.cmake" -Destination $CmakeInstallDir -Force

cmake --install "$CmakeInstallDir"

Remove-Item "$CmakeInstallDir\cmake_install.cmake" -Force
