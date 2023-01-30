param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$FoxDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistDELauncherBin"
Write-Host "GUI Launcher: Copy Program to '$TargetDir'"

Install-Files -Path "$OutputDir\delauncher-gui.exe" -Destination $TargetDir


# dependencies
Write-Host "GUI Launcher: Copy Dependencies to '$TargetDir'"

Copy-Files -SourceDir $FoxDir -TargetDir $TargetDir -Pattern "*.dll"


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbLauncherBin"
Write-Host "GUI Launcher: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\delauncher-gui.pdb" -Destination $TargetDir
