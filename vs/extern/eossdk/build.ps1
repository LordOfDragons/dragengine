param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "eossdk"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

Expand-Archive -Path "$SourceDir\eossdk.zip" -DestinationPath "$ProjectDir\eossdk"
Expand-TarXz -Path "$SourceDir\eossdk_bin_windows.tar.xz" -Destination "$ProjectDir\eossdk"
