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

$LibVersion = "main"
$BuildDir = Join-Path -Path $ProjectDir -ChildPath "build"
$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libyuv-vc64"

# Clean up old builds
if (Test-Path $BuildDir) {
    Remove-Item $BuildDir -Force -Recurse
}

if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

# Extract source archive
Push-Location $ProjectDir
Expand-TarXz -Path "$SourceDir\libyuv-$LibVersion.tar.xz" -Destination $BuildDir
Pop-Location

$CmakeSourceDir = Join-Path -Path $BuildDir -ChildPath "libyuv-$LibVersion"
$CmakeBuildDir = Join-Path -Path $BuildDir -ChildPath "cmake_build"
$CmakeInstallDir = Join-Path -Path $BuildDir -ChildPath "cmake_install"

if (!(Test-Path $CmakeBuildDir)) {
    New-Item -Path $CmakeBuildDir -ItemType "directory" -Force | Out-Null
}

# Configure and build with CMake
cmake -S "$CmakeSourceDir" -B "$CmakeBuildDir" `
    -DCMAKE_INSTALL_PREFIX="$CmakeInstallDir" `
    -DCMAKE_BUILD_TYPE=Release `
    -DBUILD_SHARED_LIBS=Off `
    -DCMAKE_SYSTEM_NAME=Windows `
    -DCMAKE_SYSTEM_PROCESSOR=AMD64

cmake --build "$CmakeBuildDir" --config Release -j 8

# Install to CMake install directory
if (!(Test-Path $CmakeInstallDir)) {
    New-Item -Path $CmakeInstallDir -ItemType "directory" -Force | Out-Null
}

cmake --install "$CmakeBuildDir" --config Release --prefix "$CmakeInstallDir"

# Create the expected output structure: libyuv-vc64/include and /lib
New-Item -Path $ExpandedDir -ItemType "directory" -Force | Out-Null
New-Item -Path "$ExpandedDir\include" -ItemType "directory" -Force | Out-Null
New-Item -Path "$ExpandedDir\lib" -ItemType "directory" -Force | Out-Null

# Copy headers
Copy-Item -Path "$CmakeInstallDir\include\*" -Destination "$ExpandedDir\include" -Recurse -Force

# Copy libraries - look for .lib files in various possible locations
$PossibleLibPaths = @(
    "$CmakeBuildDir\Release",
    "$CmakeBuildDir\lib\Release",
    "$CmakeBuildDir\lib",
    "$CmakeInstallDir\lib"
)

foreach ($LibPath in $PossibleLibPaths) {
    if (Test-Path $LibPath) {
        Get-ChildItem -Path $LibPath -Filter "*.lib" -Recurse | ForEach-Object {
            Copy-Item -Path $_.FullName -Destination "$ExpandedDir\lib" -Force
        }
    }
}

# Clean up build directories
Remove-Item -Path $BuildDir -Force -Recurse
