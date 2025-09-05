param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = "$ProjectDir\build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$OpenALVersion = "1.24.2"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "openal-soft-$OpenALVersion.tar.xz" -UrlPath "openal"

Expand-TarXz -Path "$ProjectDir\openal-soft-$OpenALVersion.tar.xz" -Destination $ExpandedDir

$CmakeBuildDir = "$ExpandedDir\build"
$CmakeSourceDir = "$ExpandedDir\openal-soft-$OpenALVersion"
$CmakeInstallDir = "$ExpandedDir\install"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DALSOFT_CONFIG=OFF `
	-DALSOFT_HRTF_DEFS=OFF `
	-DALSOFT_NO_CONFIG_UTIL=ON `
	-DALSOFT_EXAMPLES=OFF `
	-DALSOFT_STATIC_LIBGCC=OFF `
	-DALSOFT_TESTS=OFF `
	-DALSOFT_UTILS=OFF `
	-DALSOFT_INSTALL=ON `
	-DALSOFT_EAX=ON `
	-DCMAKE_BUILD_TYPE=Release `
	-DALSOFT_REQUIRE_DSOUND=ON `
	-DALSOFT_REQUIRE_WINMM=ON `
	-DALSOFT_REQUIRE_WASAPI=ON `
	-DCMAKE_POSITION_INDEPENDENT_CODE=ON `
	-DLIBTYPE=STATIC

cmake --build "$CmakeBuildDir" -- /p:Configuration=Release
cmake --install "$CmakeBuildDir"

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}
if (!(Test-Path "$CmakeInstallDir\include")) {
    New-Item -Path "$CmakeInstallDir\include" -ItemType "directory"
}
if (!(Test-Path "$CmakeInstallDir\include\AL")) {
    New-Item -Path "$CmakeInstallDir\include\AL" -ItemType "directory"
}
if (!(Test-Path "$CmakeInstallDir\lib")) {
    New-Item -Path "$CmakeInstallDir\lib" -ItemType "directory"
}

Move-Item -Path "$CmakeSourceDir\include\AL\*.h" `
	-Destination "$CmakeInstallDir\include\AL"

Move-Item -Path "$CmakeBuildDir\Release\OpenAL32.lib" `
	-Destination "$CmakeInstallDir\lib"
