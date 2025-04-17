param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$true)][string]$BuildVersion
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


# build
$ArchiveName = "blender-addon-dragengine-$BuildVersion.zip"
$ArchivePath = "$OutputDir\tools\blender\$ArchiveName"

Write-Host "Blender Scripts: Compress to '$ArchivePath'"

Compress-Archive -Path "$SourceDir\dragengine-import-export" -DestinationPath $ArchivePath -Force


# application
$DataTargetDir = "$DistributeDir\$PathDistIGDEShares\tools"

Write-Host "Blender Scripts: Copy to '$DataTargetDir'"

Install-Files -Path $ArchivePath -Destination $DataTargetDir
