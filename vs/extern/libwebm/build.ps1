param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libwebm-libwebm-1.0.0.29-vc64"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libwebm-libwebm-1.0.0.29-vc64.zip" -UrlPath "libwebm"

Expand-Archive -Path "$ProjectDir\libwebm-libwebm-1.0.0.29-vc64.zip" -DestinationPath $ProjectDir
