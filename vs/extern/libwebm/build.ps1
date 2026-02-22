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


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libwebm-libwebm-1.0.0.29-vc64"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libwebm-libwebm-1.0.0.29-vc64.zip" -UrlPath "libwebm"

Expand-Archive -Path "$ProjectDir\libwebm-libwebm-1.0.0.29-vc64.zip" -DestinationPath $ProjectDir
