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

$LibVersion = "1.4.1"
$BuildDir = Join-Path -Path $ProjectDir -ChildPath "build"
$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libdav1d-$LibVersion-vc64"

# Clean up old builds
if (Test-Path $BuildDir) {
    Remove-Item $BuildDir -Force -Recurse
}

if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

# Extract source archive
Push-Location $ProjectDir
Expand-TarXz -Path "$SourceDir\libdav1d-$LibVersion.tar.xz" -Destination $BuildDir
Pop-Location

$MesonSourceDir = Join-Path -Path $BuildDir -ChildPath "libdav1d-$LibVersion"
$MesonBuildDir = Join-Path -Path $BuildDir -ChildPath "meson_build"
$MesonInstallDir = Join-Path -Path $BuildDir -ChildPath "meson_install"

if (!(Test-Path $MesonBuildDir)) {
    New-Item -Path $MesonBuildDir -ItemType "directory" -Force | Out-Null
}

# Check if Meson and Ninja are available
$MesonFound = $null -ne (Get-Command meson -ErrorAction SilentlyContinue)
$NinjaFound = $null -ne (Get-Command ninja -ErrorAction SilentlyContinue)

if (-not $MesonFound) {
    Write-Host "ERROR: Meson not found in PATH. Please install Meson (pip install meson)"
    exit 1
}

if (-not $NinjaFound) {
    Write-Host "ERROR: Ninja not found in PATH. Please install Ninja"
    exit 1
}

# Configure with Meson
meson setup "$MesonBuildDir" "$MesonSourceDir" `
    --prefix="$MesonInstallDir" `
    --buildtype=release `
    -Denable_tests=false `
    -Denable_docs=false `
    -Denable_tools=false `
    -Ddefault_library=static

# Build with Ninja
ninja -C "$MesonBuildDir" -j 8 || exit 1

# Install
ninja -C "$MesonBuildDir" install || exit 1

# Create the expected output structure: libdav1d-1.4.1-vc64/include and /lib
New-Item -Path $ExpandedDir -ItemType "directory" -Force | Out-Null
New-Item -Path "$ExpandedDir\include" -ItemType "directory" -Force | Out-Null
New-Item -Path "$ExpandedDir\lib" -ItemType "directory" -Force | Out-Null

# Copy headers
if (Test-Path "$MesonInstallDir\include") {
    Copy-Item -Path "$MesonInstallDir\include\*" -Destination "$ExpandedDir\include" -Recurse -Force
}

# Copy libraries - look for .lib files
$PossibleLibPaths = @(
    "$MesonBuildDir\src\lib\release",
    "$MesonBuildDir\src\lib",
    "$MesonInstallDir\lib",
    "$MesonInstallDir\lib\x86_64-w64-mingw32"
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
