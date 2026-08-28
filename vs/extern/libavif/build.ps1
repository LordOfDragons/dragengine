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

$LibVersion = "1.4.2"

$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libavif-$LibVersion.tar.xz" -UrlPath "libavif"

Expand-TarXz -Path "$ProjectDir\libavif-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = Join-Path -Path $ExpandedDir -ChildPath "libavif-$LibVersion"
$CmakeBuildDir = Join-Path -Path $ExpandedDir -ChildPath "build"
$CmakeInstallDir = Join-Path -Path $ExpandedDir -ChildPath "install"

$CmakeLibYuvLibDir = Join-Path -Path $ProjectDir -ChildPath "..\libyuv\build\install\lib"
$CmakeLibDav1dLibDir = Join-Path -Path $ProjectDir -ChildPath "..\libdav1d\build\install\lib"
$CmakeLibSvtav1LibDir = Join-Path -Path $ProjectDir -ChildPath "..\svtav1\build\install\lib"

if (!(Test-Path $CmakeBuildDir)) {
    New-Item -Path $CmakeBuildDir -ItemType "directory" -Force | Out-Null
}

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
    -DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
    -DCMAKE_BUILD_TYPE=Release `
    -DBUILD_SHARED_LIBS=Off `
    -DAVIF_ENABLE_GTEST=Off `
    -DAVIF_ENABLE_TESTS=Off `
    -DAVIF_ENABLE_EXAMPLES=Off `
    -DAVIF_ENABLE_FUZZERS=Off `
    -DAVIF_ENABLE_LIBYUV=On `
    -DAVIF_CODEC_DAV1D=SYSTEM `
    -DAVIF_CODEC_SVT=SYSTEM `
    -DCMAKE_SYSTEM_NAME=Windows `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5" `
    -DCMAKE_SYSTEM_PROCESSOR=AMD64 `
    -DCMAKE_PREFIX_PATH="$CmakeLibYuvLibDir;$CmakeLibDav1dLibDir;$CmakeLibSvtav1LibDir" `
    -DLIBYUV_INCLUDE_DIR="$ProjectDir\..\libyuv\build\install\include" `
    -DDAV1D_INCLUDE_DIR="$ProjectDir\..\libdav1d\build\install\include" `
    -DSVT_INCLUDE_DIR="$ProjectDir\..\svtav1\build\install\include"

cmake --build "$CmakeBuildDir" --config Release -j 8

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory" -Force | Out-Null
}

cmake --install "$CmakeBuildDir" --config Release --prefix "$CmakeInstallDir"
