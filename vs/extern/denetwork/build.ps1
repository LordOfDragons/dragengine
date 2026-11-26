param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$DENetworkVersion = "1.2"

$ExpandedDir = "$ProjectDir\build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$ArchiveFile = "$ProjectDir\DENetworkSDK-$DENetworkVersion.zip"
$BaseUrl = "https://github.com/LordOfDragons/denetwork/releases/download/v$DENetworkVersion"

if (!(Test-Path $ArchiveFile)) {
    Invoke-WebRequestWithRetry -Uri "$BaseUrl/DENetworkSDK-$DENetworkVersion.zip" -OutFile $ArchiveFile
}

# workaround for powershell bug under github
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::ExtractToDirectory($ArchiveFile, $ExpandedDir)
