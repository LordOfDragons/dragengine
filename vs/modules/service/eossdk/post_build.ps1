param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$SolutionDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\service\eossdk\$Version"

Write-Host "EOSSDK Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\service\eossdk\srveossdk.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version

$Library = "$SolutionDir\extern\eossdk\eossdk\bin\EOSSDK-Win64-Shipping.dll"
Install-Files -Path $Library -Destination $TargetDir

$DataTargetDir = "$OutputDir\$PathDistDESharesModules\service\eossdk\$Version"
Write-Host "EOSSDK Module: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir $DataTargetDir -Pattern "*.*"


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\service\eossdk\$Version"
Write-Host "EOSSDK Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\service\eossdk\srveossdk.pdb" -Destination $TargetDir
