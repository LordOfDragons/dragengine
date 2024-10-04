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

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "eossdk.zip" -UrlPath "eossdk"
DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "eossdk_bin_windows.tar.xz" -UrlPath "eossdk"

Expand-Archive -Path "$ProjectDir\eossdk.zip" -DestinationPath "$ProjectDir\eossdk"
Expand-TarXz -Path "$ProjectDir\eossdk_bin_windows.tar.xz" -Destination "$ProjectDir\eossdk"
