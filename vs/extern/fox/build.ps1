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


$LibVersion = "1.7.85"

$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "fox-$LibVersion-bin"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "fox-$LibVersion-bin.tar.xz" -UrlPath "fox"

Expand-TarXz -Path "$ProjectDir\fox-$LibVersion-bin.tar.xz" -Destination $ProjectDir
