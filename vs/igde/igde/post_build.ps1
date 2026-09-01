param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


# application
$TargetDir = "$DistributeDir\$PathDistIGDEBin"

$BaseDir = "$OutputDir\deigde"

Write-Host "Igde App: Copy Program to '$TargetDir'"
Install-Files -Path "$BaseDir\deigde.exe" -Destination $TargetDir

$DataTargetDir = "$DistributeDir\$PathDistIGDEShares"
Write-Host "Igde App: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir $DataTargetDir -Pattern "*"

Write-Host "Igde App: Copy VC Runtime to '$TargetDir'"

Copy-Files -SourceDir $BaseDir -TargetDir $TargetDir -Pattern "*.dll"


# debug
$TargetDir = "$DistributeDir\$PathDistIGDEPdbBin"
Write-Host "Igde Debug: Copy PDBs to '$TargetDir'"

Install-Files -Path "$BaseDir\deigde.pdb" -Destination $TargetDir
