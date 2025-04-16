param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path "$SourceDir\..\SConscript"

$TargetDir = "$DistributeDir\$PathDistIGDEDataModules\particleemitter"

Write-Host "Particle Emitter Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\particleemitter\particleemitter.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml"`
    -Library $Library -Version $Version


$DataTargetDir = "$DistributeDir\$PathDistIGDESharesModules\particleemitter"
Write-Host "Particle Emitter Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$DistributeDir\$PathDistIGDEPdbDataModules\particleemitter"
Write-Host "Particle Emitter Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\particleemitter\particleemitter.pdb" -Destination $TargetDir
