﻿param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# application
$TargetDir = "$DistributeDir\$PathDistDELauncherBin"
Write-Host "Console Launcher: Copy Program to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\console\delauncher-console.exe" -Destination $TargetDir


# debug
$TargetDir = "$DistributeDir\$PathDistDEPdbLauncherBin"
Write-Host "Console Launcher: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\console\delauncher-console.pdb" -Destination $TargetDir
