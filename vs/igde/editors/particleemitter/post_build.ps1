param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = Join-Path -Path $OutputDir -ChildPath "$PathDistIgdeEditors\particleemitter\$Version"

Write-Host "Particle Emitter Editor: Copy Module to '$TargetDir'"

$Library = Join-Path -Path $OutputDir -ChildPath "particleemitter.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


# debug
$TargetDir = Join-Path -Path $OutputDir -ChildPath $PathDistDEPdbModules
Write-Host "Particle Emitter Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "particleemitter.pdb") -Destination $TargetDir
