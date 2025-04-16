param (
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistDEDataModules"

Write-Host "DEModules: Copy Module to '$TargetDir'"

$Library = "$OutputDir\demodules\demodules.dll"
Install-Files -Path $Library -Destination $TargetDir


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules"
Write-Host "DEModules: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\demodules\demodules.pdb" -Destination $TargetDir
