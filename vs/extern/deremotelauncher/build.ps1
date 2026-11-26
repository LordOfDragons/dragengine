param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

$DERemoteLauncherVersion = "1.1"

$ExpandedDir = "$ProjectDir\build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$ArchiveFile = "$ProjectDir\DERemoteLauncherSDK-$DERemoteLauncherVersion.zip"
$BaseUrl = "https://github.com/LordOfDragons/deremotelauncher/releases/download/v$DERemoteLauncherVersion"

if (!(Test-Path $ArchiveFile)) {
    Invoke-WebRequestWithRetry -Uri "$BaseUrl/DERemoteLauncherSDK-$DERemoteLauncherVersion.zip" -OutFile $ArchiveFile
}

# workaround for powershell bug under github
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::ExtractToDirectory($ArchiveFile, $ExpandedDir)
