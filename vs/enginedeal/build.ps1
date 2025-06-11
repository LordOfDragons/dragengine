param (
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$true)][string]$BuildVersion
)

Import-Module "$PSScriptRoot\..\shared.psm1"


# build
$TempArchivePath = "$OutputDir\enginedeal\temp.zip"
$ArchiveName = "dragengine-$BuildVersion.deal"
$ArchivePath = "$OutputDir\enginedeal\$ArchiveName"
$SourcePath = "$OutputDir\..\enginedeal"

Write-Host "Engine Assets: Compress to '$ArchivePath'"

if (!(Test-Path $ArchivePath)) {
    New-Item -ItemType Directory $ArchivePath | Out-Null
}

Compress-Archive -Path "$SourcePath\*" -DestinationPath $TempArchivePath -Force

if (Test-Path $ArchivePath) {
    Remove-Item $ArchivePath -Force
}
Rename-Item -Path $TempArchivePath -NewName $ArchiveName


# application
$DataTargetDir = "$DistributeDir\$PathDistDEShares"

Write-Host "Engine Assets: Copy to '$DataTargetDir'"

Install-Files -Path $ArchivePath -Destination $DataTargetDir
