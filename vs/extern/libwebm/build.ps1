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

$LibVersion = "1.0.0.32"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libwebm-libwebm-$LibVersion.tar.xz" -UrlPath "libwebm"

Expand-TarXz -Path "$ProjectDir\libwebm-libwebm-$LibVersion.tar.xz" -Destination $ExpandedDir

# patch master_value_parser.h to fix MSVC C2248 error. since this is just an internal build
# nobody is going to get to compile against just replace all "private:" with "public:".
# ugly but enough to get it compiling
$MasterValueParserHeader = "$ExpandedDir\libwebm-libwebm-$LibVersion\webm_parser\src\master_value_parser.h"
$Content = Get-Content -Path $MasterValueParserHeader -Raw
$Content = $Content -replace ' private:', ' public:'
Set-Content -Path $MasterValueParserHeader -Value $Content -NoNewline

$CmakeSourceDir = "$ExpandedDir\libwebm-libwebm-$LibVersion"
$CmakeBuildDir = "$ExpandedDir\build"
$CmakeInstallDir = "$ExpandedDir\install"

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DBUILD_SHARED_LIBS=Off `
	-DENABLE_WEBMTS=Off `
	-DCMAKE_SYSTEM_NAME=Windows `
	-DCMAKE_SYSTEM_PROCESSOR=AMD64 `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5" `
	-DENABLE_WEBM_PARSER=On

cmake --build "$CmakeBuildDir" -j 8 -- /property:Configuration=Release

if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory"
}

Copy-Item "$CmakeBuildDir\cmake_install.cmake" -Destination $CmakeInstallDir -Force

cmake --install "$CmakeInstallDir"

Remove-Item "$CmakeInstallDir\cmake_install.cmake" -Force
