param (
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\shared.psm1"

# application
$TargetDir = "$DistributeDir\$PathDistDEDataModules"

Write-Host "DEModules: Copy Module to '$TargetDir'"

$Library = "$OutputDir\demodules\demodules.dll"
Install-Files -Path $Library -Destination $TargetDir


# debug
$TargetDir = "$DistributeDir\$PathDistDEPdbDataModules"
Write-Host "DEModules: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\demodules\demodules.pdb" -Destination $TargetDir
