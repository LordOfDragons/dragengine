param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = "$ProjectDir\DERemoteLauncherSDK"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$ArchiveFile = "$ProjectDir\DERemoteLauncherSDK-nightly.zip"
$BaseUrl = "https://github.com/LordOfDragons/deremotelauncher/releases/download/nightly"

if (!(Test-Path $ArchiveFile)) {
    Invoke-WebRequest "$BaseUrl/DERemoteLauncherSDK-nightly.zip" -OutFile $ArchiveFile
}

Expand-Archive -Path $ArchiveFile -DestinationPath $ProjectDir
