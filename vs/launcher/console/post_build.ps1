param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistDELauncherBin"
Write-Host "Console Launcher: Copy Program to '$TargetDir'"

Install-Files -Path "$OutputDir\delauncher-console.exe" -Destination $TargetDir


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbLauncherBin"
Write-Host "Console Launcher: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\delauncher-console.pdb" -Destination $TargetDir
