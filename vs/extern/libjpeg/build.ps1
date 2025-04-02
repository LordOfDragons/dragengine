param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$LibVersion = "3.1.0"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libjpeg-turbo-$LibVersion.tar.xz" -UrlPath "libjpeg"

Expand-TarXz -Path "$ProjectDir\libjpeg-turbo-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = "$ExpandedDir\libjpeg-turbo-$LibVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

# CMAKE_SYSTEM_NAME, CMAKE_SYSTEM_PROCESSOR:
#   libjpeg fails if system name is not this value

# CMAKE_POLICY_VERSION_MINIMUM:
#   workaround for github build problem

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DENABLE_SHARED=Off `
	-DENABLE_STATIC=On `
	-DWITH_TURBOJPEG=Off `
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
