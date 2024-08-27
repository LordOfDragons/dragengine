param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

$ExpandedDir = Join-Path -Path $SourceDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "modio-sdk.tar.xz" -UrlPath "modio"
DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "modio-sdk-ext.tar.xz" -UrlPath "modio"

Expand-TarXz -Path "$ProjectDir\modio-sdk.tar.xz" -Destination $ExpandedDir
Expand-TarXz -Path "$ProjectDir\modio-sdk-ext.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = Join-Path -Path $ExpandedDir -ChildPath "modio-sdk"
$CmakeBuildDir = Join-Path -Path $ExpandedDir -ChildPath "build"
$CmakeInstallDir = Join-Path -Path $ExpandedDir -ChildPath "install"

$Env:CXXFLAGS = "/DMODIO_SEPARATE_COMPILATION"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" --preset=win

cmake --build "$CmakeBuildDir" -- -j 8

# modio build script has a bug. if build and install directory are custom
# the install script tries to access 'cmake_install.cmake' inside 'install dir'
# instead of 'build dir'. copy it over to prevent the script from failing
if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}

Copy-Item "$CmakeBuildDir\cmake_install.cmake" `
    -Destination $CmakeInstallDir -Force

cmake --install "$CmakeInstallDir"

# the bug mentioned above also copies the files into the totally wrong directory.
# move them to where they belong
Move-Item -Path "$CmakeSourceDir\out\install\win\*" `
    -Destination "$CmakeInstallDir"

Remove-Item "$CmakeSourceDir\out" -Force -Recurse
Remove-Item "$CmakeInstallDir\cmake_install.cmake" -Force
