param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\graphic\opengl\graopengl.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


$DataTargetDir = "$OutputDir\$PathDistDESharesModules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -Pattern "*" -TargetDir $DataTargetDir


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\graphic\opengl\graopengl.pdb" -Destination $TargetDir
