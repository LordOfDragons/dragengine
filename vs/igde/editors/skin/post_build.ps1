﻿param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path "$SourceDir\..\SConscript"

$TargetDir = "$DistributeDir\$PathDistIGDEDataModules\skin"

Write-Host "Skin Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\skin\skin.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml"`
    -Library $Library -Version $Version


$DataTargetDir = "$DistributeDir\$PathDistIGDESharesModules\skin"
Write-Host "Skin Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$DistributeDir\$PathDistIGDEPdbDataModules\skin"
Write-Host "Skin Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\skin\skin.pdb" -Destination $TargetDir
