param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$FoxDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$TargetDir = "$OutputDir\$PathDistIGDEDataModules\project"

Write-Host "Project Editor TestRunner: Copy Program to '$TargetDir'"

$BaseDir = "$OutputDir\igde_editor\project_testrunner"

$Program = "$BaseDir\testrunner.exe"
Install-Files -Path $Program -Destination $TargetDir

Write-Host "Project Editor TestRunner: Copy VC Runtime to '$TargetDir'"

Copy-Files -SourceDir $BaseDir -TargetDir $TargetDir -Pattern "vcruntime*.dll"


# dependencies
# NOTE the fox dependency is only required for the crash recovery module since
#      it requires the library to be present in the application directory
$TargetDir = "$OutputDir\$PathDistIGDEDataModules\project"
Write-Host "Project Editor TestRunner: Copy Dependencies to '$TargetDir'"

Copy-Files -SourceDir $FoxDir -TargetDir $TargetDir -Pattern "*.dll"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbDataModules"
Write-Host "Project Editor TestRunner: Copy PDBs to '$TargetDir'"

Install-Files -Path "$BaseDir\testrunner.pdb" -Destination $TargetDir
