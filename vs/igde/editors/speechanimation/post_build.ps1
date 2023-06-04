param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path "$SourceDir\..\SConscript"

$TargetDir = "$OutputDir\$PathDistIGDEDataModules\speechanimation"

Write-Host "Speech Animation Editor: Copy Module to '$TargetDir'"

$Library = "$OutputDir\igde_editor\speechanimation\speechanimation.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml"`
    -Library $Library -Version $Version


$DataTargetDir = "$OutputDir\$PathDistIGDESharesModules\speechanimation"
Write-Host "Speech Animation Editor: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir" -Pattern "*"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbDataModules\speechanimation"
Write-Host "Speech Animation Editor: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\igde_editor\speechanimation\speechanimation.pdb" -Destination $TargetDir
