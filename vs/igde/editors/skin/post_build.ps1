param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistIGDEDataModules\skin"

Write-Host "Skin Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\skin\skin.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml" -Library $Library


$DataTargetDir = "$OutputDir\$PathDistIGDESharesModules\skin"
Write-Host "Skin Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbDataModules\skin"
Write-Host "Skin Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\skin\skin.pdb" -Destination $TargetDir
