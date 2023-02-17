param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\image\jpeg\$Version"

Write-Host "JPEG Module: Copy Module to '$TargetDir'"

$Library = Join-Path -Path $OutputDir -ChildPath "imgjpeg.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\image\jpeg\$Version"
Write-Host "JPEG Module: Copy PDBs to '$TargetDir'"

Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "imgjpeg.pdb") -Destination $TargetDir
