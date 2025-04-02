param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

$ExpandedDir = "$ProjectDir\build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$OpenXRVersion = "1.1.46"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "OpenXR-SDK-release-$OpenXRVersion.tar.xz" -UrlPath "openxr"

Expand-TarXz -Path "$ProjectDir\OpenXR-SDK-release-$OpenXRVersion.tar.xz" -Destination $ExpandedDir

$CmakeBuildDir = "$ExpandedDir\build"
$CmakeSourceDir = "$ExpandedDir\OpenXR-SDK-release-$OpenXRVersion"
$CmakeInstallDir = "$ExpandedDir\install"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeBuildDir" `
	-DCMAKE_INSTALL_LIBDIR="$CmakeBuildDir\lib" `
	-DCMAKE_BUILD_TYPE=Release `
	-DBUILD_API_LAYERS=OFF `
	-DBUILD_TESTS=OFF `
	-DBUILD_WITH_STD_FILESYSTEM=OFF `
	-DBUILD_WITH_SYSTEM_JSONCPP=OFF `
	-DBUILD_WITH_WAYLAND_HEADERS=OFF `
	-DBUILD_WITH_XCB_HEADERS=OFF `
	-DDYNAMIC_LOADER=OFF `
	-Wno-dev

cmake --build "$CmakeBuildDir" -- /p:Configuration=Release
cmake --install "$CmakeBuildDir"

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}
if (!(Test-Path "$CmakeInstallDir\include")) {
    New-Item -Path "$CmakeInstallDir\include" -ItemType "directory"
}
if (!(Test-Path "$CmakeInstallDir\include\openxr")) {
    New-Item -Path "$CmakeInstallDir\include\openxr" -ItemType "directory"
}
if (!(Test-Path "$CmakeInstallDir\lib")) {
    New-Item -Path "$CmakeInstallDir\lib" -ItemType "directory"
}

Move-Item -Path "$CmakeBuildDir\include\openxr\*.h" `
	-Destination "$CmakeInstallDir\include\openxr"

Move-Item -Path "$CmakeBuildDir\lib\openxr_loader.lib" `
	-Destination "$CmakeInstallDir\lib"
