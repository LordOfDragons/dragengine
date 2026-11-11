param (
	[Parameter(Mandatory=$true)][string]$ProjectDir,
	[Parameter(Mandatory=$true)][string]$SourceDir,
	[Parameter(Mandatory=$true)][string]$OutputDir,
	[Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

$LibVersion = "1.13"

$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
	Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "msdfgen-$LibVersion.tar.xz" -UrlPath "msdfgen"

Expand-TarXz -Path "$ProjectDir\msdfgen-$LibVersion.tar.xz" -Destination $ExpandedDir

$CmakeSourceDir = Join-Path -Path $ExpandedDir -ChildPath "msdfgen-$LibVersion"
$CmakeBuildDir = Join-Path -Path $ExpandedDir -ChildPath "build"
$CmakeInstallDir = Join-Path -Path $ExpandedDir -ChildPath "install"

if (!(Test-Path $CmakeBuildDir)) {
	New-Item -Path $CmakeBuildDir -ItemType "directory" -Force | Out-Null
}

cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
	-DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
	-DCMAKE_BUILD_TYPE=Release `
	-DCMAKE_POLICY_VERSION_MINIMUM="3.5" `
	-DCMAKE_INSTALL_LIBDIR=lib `
	-DBUILD_SHARED_LIBS=OFF `
	-DMSDFGEN_CORE_ONLY=ON `
	-DMSDFGEN_BUILD_STANDALONE=OFF `
	-DMSDFGEN_INSTALL=ON

cmake --build "$CmakeBuildDir" --config Release -j 8

if (!(Test-Path $CmakeInstallDir)) {
	New-Item -Path $CmakeInstallDir -ItemType "directory" -Force | Out-Null
}

cmake --install "$CmakeBuildDir" --config Release --prefix "$CmakeInstallDir"

# core headers are not installed by the core-only build
$CoreSourceDir = Join-Path -Path $CmakeSourceDir -ChildPath "core"
$CoreIncludeDir = Join-Path -Path $CmakeInstallDir -ChildPath "include\msdfgen\core"
if (!(Test-Path $CoreIncludeDir)) {
	New-Item -Path $CoreIncludeDir -ItemType "directory" -Force | Out-Null
}
Get-ChildItem -Path $CoreSourceDir -File `
	| Where-Object { $_.Extension -in '.h', '.hpp' } `
	| Copy-Item -Destination $CoreIncludeDir -Force
