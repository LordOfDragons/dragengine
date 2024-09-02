param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libjpeg-turbo-2.0.3-vc64"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libjpeg-turbo-2.0.3-vc64.zip" -UrlPath "libjpeg"

Expand-Archive -Path "$ProjectDir\libjpeg-turbo-2.0.3-vc64.zip" -DestinationPath $ProjectDir
