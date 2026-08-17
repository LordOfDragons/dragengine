param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


# libsharpyuv is built as part of libwebp, just copy the built libraries
$WebpInstallDir = Join-Path -Path $ProjectDir -ChildPath "..\libwebp\build\install"

if (Test-Path $WebpInstallDir) {
    # Copy the sharpyuv library to this project's expected location
    $ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libwebp-1.3.2-vc64"
    if (Test-Path $ExpandedDir) {
        Remove-Item $ExpandedDir -Force -Recurse
    }
    New-Item -Path $ExpandedDir -ItemType "directory" -Force | Out-Null
    Copy-Item "$WebpInstallDir\*" -Destination $ExpandedDir -Recurse -Force
}
