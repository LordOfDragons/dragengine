param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\image\png3d\$Version"

Write-Host "PNG3D Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\image\png3d\imgpng3d.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\image\png3d\$Version"
Write-Host "PNG3D Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\image\png3d\imgpng3d.pdb" -Destination $TargetDir
