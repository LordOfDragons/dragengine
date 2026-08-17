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


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$LibVersion = "1.14.1"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libvpx-v$LibVersion.tar.xz" -UrlPath "libvpx"

Expand-TarXz -Path "$ProjectDir\libvpx-v$LibVersion.tar.xz" -Destination $ExpandedDir

$SourceDir_Vpx = "$ExpandedDir\libvpx-v$LibVersion"
$BuildDir_Vpx = "$ExpandedDir\build_vpx"
$InstallDir_Vpx = "$ExpandedDir\install"

if (!(Test-Path $BuildDir_Vpx)) {
    New-Item -Path $BuildDir_Vpx -ItemType "directory" -Force | Out-Null
}

# libvpx build using configure and nmake for Windows
$OldDir = Get-Location
Set-Location $BuildDir_Vpx

# Run configure script for Windows Visual Studio
& cmd /c "$SourceDir_Vpx\configure" --target=x86_64-win64-vs16 --enable-static --disable-shared --disable-examples --disable-docs --disable-unit-tests 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Warning "libvpx configure had issues but continuing..."
}

# Build using nmake
& nmake 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Warning "libvpx nmake had issues but continuing..."
}

Set-Location $OldDir

# Create install directory structure
if (!(Test-Path $InstallDir_Vpx)) {
    New-Item -Path $InstallDir_Vpx -ItemType "directory" -Force | Out-Null
}
if (!(Test-Path "$InstallDir_Vpx\lib")) {
    New-Item -Path "$InstallDir_Vpx\lib" -ItemType "directory" -Force | Out-Null
}
if (!(Test-Path "$InstallDir_Vpx\include")) {
    New-Item -Path "$InstallDir_Vpx\include" -ItemType "directory" -Force | Out-Null
}

# Copy built library files
Get-ChildItem "$BuildDir_Vpx\*.lib" -ErrorAction SilentlyContinue | ForEach-Object {
    Copy-Item $_ -Destination "$InstallDir_Vpx\lib" -Force
}

# Copy header files
if (Test-Path "$SourceDir_Vpx\vpx") {
    Copy-Item "$SourceDir_Vpx\vpx\*" -Destination "$InstallDir_Vpx\include\vpx" -Recurse -Force -ErrorAction SilentlyContinue
}
