param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "zlib-1.2.13-vc64"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "zlib-1.2.13-vc64.zip" -UrlPath "zlib"

Expand-Archive -Path "$ProjectDir\zlib-1.2.13-vc64.zip" -DestinationPath $ProjectDir
