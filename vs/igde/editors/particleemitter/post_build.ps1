param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path "$SourceDir\..\SConscript"

$TargetDir = "$OutputDir\$PathDistIGDEDataModules\particleemitter"

Write-Host "Particle Emitter Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\particleemitter\particleemitter.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml"`
    -Library $Library -Version $Version


$DataTargetDir = "$OutputDir\$PathDistIGDESharesModules\particleemitter"
Write-Host "Particle Emitter Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbDataModules\particleemitter"
Write-Host "Particle Emitter Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\particleemitter\particleemitter.pdb" -Destination $TargetDir
