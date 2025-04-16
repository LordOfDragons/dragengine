param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path "$SourceDir\..\SConscript"

$TargetDir = "$DistributeDir\$PathDistIGDEDataModules\synthesizer"

Write-Host "Synthesizer Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\synthesizer\synthesizer.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml"`
    -Library $Library -Version $Version


$DataTargetDir = "$DistributeDir\$PathDistIGDESharesModules\synthesizer"
Write-Host "Synthesizer Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$DistributeDir\$PathDistIGDEPdbDataModules\synthesizer"
Write-Host "Synthesizer Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\synthesizer\synthesizer.pdb" -Destination $TargetDir
