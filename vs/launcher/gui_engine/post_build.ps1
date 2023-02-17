param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistDELauncherBin"
Write-Host "GUI Launcher Engine: Copy Program to '$TargetDir'"

Install-Files -Path "$OutputDir\delauncher-gui-engine.exe" -Destination $TargetDir


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbLauncherBin"
Write-Host "GUI Launcher Engine: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\delauncher-gui-engine.pdb" -Destination $TargetDir
