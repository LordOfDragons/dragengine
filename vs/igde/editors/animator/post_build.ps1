﻿param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistIGDEDataModules\animator"

Write-Host "Animator Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\animator\animator.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml" -Library $Library


$DataTargetDir = "$OutputDir\$PathDistIGDESharesModules\animator"
Write-Host "Animator Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbDataModules\animator"
Write-Host "Animator Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\animator\animator.pdb" -Destination $TargetDir
