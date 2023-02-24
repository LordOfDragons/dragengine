param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistIGDEDataModules\sky"

Write-Host "Sky Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\sky\sky.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml" -Library $Library


$DataTargetDir = "$OutputDir\$PathDistIGDESharesModules\sky"
Write-Host "Sky Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbDataModules"
Write-Host "Sky Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\sky\sky.pdb" -Destination $TargetDir
