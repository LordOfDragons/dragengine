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


# libvorbisfile is built as part of libvorbis, just copy the built libraries
$VorbisInstallDir = Join-Path -Path $ProjectDir -ChildPath "..\libvorbis\build\install"

if (Test-Path $VorbisInstallDir) {
    # Copy the vorbisfile library to this project's expected location
    $ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libvorbis-1.3.7-vc64"
    if (Test-Path $ExpandedDir) {
        Remove-Item $ExpandedDir -Force -Recurse
    }
    New-Item -Path $ExpandedDir -ItemType "directory" -Force | Out-Null
    Copy-Item "$VorbisInstallDir\*" -Destination $ExpandedDir -Recurse -Force
}
