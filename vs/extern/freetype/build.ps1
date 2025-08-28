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

$LibVersion = "2.13.3"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "freetype-$LibVersion.tar.xz" -UrlPath "freetype"

Expand-TarXz -Path "$ProjectDir\freetype-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = "$ExpandedDir\freetype-$LibVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DENABLE_SHARED=Off `
	-DENABLE_STATIC=On `
	-DCMAKE_SYSTEM_NAME=Windows `
	-DCMAKE_SYSTEM_PROCESSOR=AMD64 `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5" `
	-DFT_DISABLE_ZLIB=Off `
	-DFT_DISABLE_BZIP2=On `
	-DFT_DISABLE_PNG=Off `
	-DFT_DISABLE_HARFBUZZ=On `
	-DFT_DISABLE_BROTLI=On

cmake --build "$CmakeBuildDir" -j 8 -- /property:Configuration=Release

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}

Copy-Item "$CmakeBuildDir\cmake_install.cmake" -Destination $CmakeInstallDir -Force

cmake --install "$CmakeInstallDir"

Remove-Item "$CmakeInstallDir\cmake_install.cmake" -Force
