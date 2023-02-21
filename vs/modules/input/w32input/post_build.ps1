param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\input\w32input\$Version"

Write-Host "Windows Input Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\input\w32input\inpw32.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version

Copy-Files -Pattern "*" -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\data")`
    -TargetDir (Join-Path -Path $OutputDir -ChildPath "$PathDistDESharesModules\input\w32input\$Version")


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\input\w32input\$Version"
Write-Host "Windows Input Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\input\w32input\inpw32.pdb" -Destination $TargetDir
