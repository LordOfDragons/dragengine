param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistIGDEBin"

Write-Host "Project Editor TestRunner: Copy Program to '$TargetDir'"

$Program = "$OutputDir\igde_editor\project\testrunner.exe"
Install-Files -Path $Program -Destination $TargetDir


# debug
$TargetDir = "$OutputDir\$PathDistIGDEBin"
Write-Host "Project Editor TestRunner: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\project\testrunner.pdb" -Destination $TargetDir
