param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$EosSdkVersion = "1.17.0"

$ExpandedDir = "$ProjectDir\build\eossdk"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "eossdk-$EosSdkVersion.zip" -UrlPath "eossdk"
DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "eossdk_bin_windows-$EosSdkVersion.tar.xz" -UrlPath "eossdk"

# workaround for powershell bug under github
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::ExtractToDirectory("$ProjectDir\eossdk-$EosSdkVersion.zip", $ExpandedDir)

Expand-TarXz -Path "$ProjectDir\eossdk_bin_windows-$EosSdkVersion.tar.xz" -Destination $ExpandedDir
