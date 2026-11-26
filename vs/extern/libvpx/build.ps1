param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "libvpx-1.12.0-vc64"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libvpx-1.12.0-vc64.zip" -UrlPath "libvpx"

# workaround for powershell bug under github
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::ExtractToDirectory("$ProjectDir\libvpx-1.12.0-vc64.zip", $ProjectDir)
