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


$EosSdkVersion = "1.17.0"

$ExpandedDir = "$ProjectDir\build\eossdk"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "eossdk-$EosSdkVersion.zip" -UrlPath "eossdk"
DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "eossdk_bin_windows-$EosSdkVersion.tar.xz" -UrlPath "eossdk"

Expand-Archive -Path "$ProjectDir\eossdk-$EosSdkVersion.zip" -DestinationPath $ExpandedDir
Expand-TarXz -Path "$ProjectDir\eossdk_bin_windows-$EosSdkVersion.tar.xz" -Destination $ExpandedDir
