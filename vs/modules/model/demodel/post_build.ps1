﻿param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\model\demodel\$Version"

Write-Host "DEModel Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\model\demodel\mdldemodel.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\model\demodel\$Version"
Write-Host "DEModel Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\model\demodel\mdldemodel.pdb" -Destination $TargetDir
