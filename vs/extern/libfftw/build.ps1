param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

if (Test-Path "$PSScriptRoot\..\..\github_cached_externals") {
    return
}

$FftwVersion = "3.3.10"
$SourceArchive = "libfftw-$FftwVersion.tar.xz"

$ExpandedDir = "$ProjectDir\build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact $SourceArchive -UrlPath "libfftw"

Expand-TarXz -Path "$ProjectDir\$SourceArchive" -Destination $ExpandedDir

$CmakeSourceDir = "$ExpandedDir\fftw-$FftwVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_POLICY_VERSION_MINIMUM=3.5 `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DDISABLE_FORTRAN=ON `
	-DBUILD_SHARED_LIBS=OFF `
	-DBUILD_TESTS=OFF `

cmake --build "$CmakeBuildDir" --config Release
cmake --install "$CmakeBuildDir" --config Release
