param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# application
$TargetDir = "$DistributeDir\$PathDistDELauncherBin"
Write-Host "GUI Launcher Engine: Copy Program to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\gui\delauncher-gui-engine.exe" -Destination $TargetDir


# debug
$TargetDir = "$DistributeDir\$PathDistDEPdbLauncherBin"
Write-Host "GUI Launcher Engine: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\gui\delauncher-gui-engine.pdb" -Destination $TargetDir
