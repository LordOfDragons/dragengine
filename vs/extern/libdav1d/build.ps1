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

$LibVersion = "1.5.4"

$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "dav1d-$LibVersion.tar.xz" -UrlPath "libdav1d"

Expand-TarXz -Path "$ProjectDir\dav1d-$LibVersion.tar.xz" -Destination $ExpandedDir

$MesonSourceDir = Join-Path -Path $ExpandedDir -ChildPath "dav1d-$LibVersion"
$MesonBuildDir = Join-Path -Path $ExpandedDir -ChildPath "build"
$MesonInstallDir = Join-Path -Path $ExpandedDir -ChildPath "install"

if (!(Test-Path $MesonBuildDir)) {
    New-Item -Path $MesonBuildDir -ItemType "directory" -Force | Out-Null
}


$MesonFound = $null -ne (Get-Command meson -ErrorAction SilentlyContinue)
$NinjaFound = $null -ne (Get-Command ninja -ErrorAction SilentlyContinue)

if (-not $MesonFound) {
    Write-Host "ERROR: Meson not found in PATH. Please install Meson"
    exit 1
}

if (-not $NinjaFound) {
    Write-Host "ERROR: Ninja not found in PATH. Please install Ninja"
    exit 1
}

$NasmStandardPath = "C:\Program Files\NASM"
$NasmChocoPath = "$env:SystemDrive\ProgramData\chocolatey\lib\nasm\tools\nasm"

if (Test-Path "$NasmStandardPath\nasm.exe") {
    $TargetNasmDir = $NasmStandardPath
} elseif (Test-Path "$NasmChocoPath\nasm.exe") {
    $TargetNasmDir = $NasmChocoPath
} else {
    Write-Host "ERROR: NASM not found. Please install NASM"
    exit 1
}

$env:PATH = "$TargetNasmDir;$env:PATH"
$env:CMAKE_ARGS = "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"

meson setup "$MesonBuildDir" "$MesonSourceDir" `
    --prefix="$MesonInstallDir" `
    --buildtype=release `
    -Denable_tests=false `
    -Denable_docs=false `
    -Denable_tools=false `
    -Ddefault_library=static

ninja -C "$MesonBuildDir" -j 8
ninja -C "$MesonBuildDir" install

Copy-Item -Path "$MesonInstallDir\lib\libdav1d.a" -Destination "$MesonInstallDir\lib\libdav1d.lib" -Force
