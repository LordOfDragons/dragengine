param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# application
$TargetDir = "$DistributeDir\$PathDistLive"
Write-Host "Live Launcher: Copy Program to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\live\LaunchWindows64.exe" -Destination $TargetDir


# dependencies
Write-Host "Live Launcher: Copy VC Runtime to '$TargetDir'"

Copy-Files -SourceDir "$OutputDir\launcher\live" -TargetDir $TargetDir -Pattern "vcruntime*.dll"


# debug
$TargetDir = "$DistributeDir\$PathDistLivePdb"
Write-Host "Live Launcher: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\live\LaunchWindows64.pdb" -Destination $TargetDir
