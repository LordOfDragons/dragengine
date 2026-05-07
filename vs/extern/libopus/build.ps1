param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

if (Test-Path "$PSScriptRoot\..\..\github_cached_externals") {
    return
}

$OpusVersion = "1.6.1"
$SourceArchive = "opus-$OpusVersion.tar.xz"

$ExpandedDir = "$ProjectDir\build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact $SourceArchive -UrlPath "libopus"

Expand-TarXz -Path "$ProjectDir\$SourceArchive" -Destination $ExpandedDir

$CmakeSourceDir = "$ExpandedDir\opus-$OpusVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DOPUS_BUILD_SHARED_LIBRARY=OFF `
	-DOPUS_BUILD_PROGRAMS=OFF `
	-DOPUS_BUILD_TESTING=OFF

cmake --build "$CmakeBuildDir" --config Release
cmake --install "$CmakeBuildDir" --config Release
