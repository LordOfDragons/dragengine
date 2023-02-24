param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


# application
$TargetDir = "$OutputDir\$PathDistIGDEBin"

Write-Host "Igde App: Copy Program to '$TargetDir'"
Install-Files -Path "$OutputDir\deigde\deigde.exe" -Destination $TargetDir

$DataTargetDir = "$OutputDir\$PathDistIGDEShares"
Write-Host "Igde App: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir $DataTargetDir -Pattern "*"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbBin"
Write-Host "Igde Debug: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\deigde\deigde.pdb" -Destination $TargetDir
