param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\image\png\$Version"

Write-Host "IES Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\image\png\imgpng.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\image\png\$Version"
Write-Host "IES Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\image\png\imgpng.pdb" -Destination $TargetDir
