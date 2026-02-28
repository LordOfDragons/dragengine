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


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libpng-1.6.29-ani-vc64"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libpng-1.6.29-ani-vc64.zip" -UrlPath "libpng"

Expand-Archive -Path "$ProjectDir\libpng-1.6.29-ani-vc64.zip" -DestinationPath $ProjectDir
