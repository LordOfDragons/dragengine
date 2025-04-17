param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = "$ProjectDir\DENetworkSDK"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$ArchiveFile = "$ProjectDir\DENetworkSDK-nightly.zip"
$BaseUrl = "https://github.com/LordOfDragons/denetwork/releases/download/nightly"

if (!(Test-Path $ArchiveFile)) {
    Invoke-WebRequest "$BaseUrl/DENetworkSDK-nightly.zip" -OutFile $ArchiveFile
}

Expand-Archive -Path $ArchiveFile -DestinationPath $ProjectDir
