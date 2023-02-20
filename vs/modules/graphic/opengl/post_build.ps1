param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy Module to '$TargetDir'"

$Library = Join-Path -Path $OutputDir -ChildPath "graopengl.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


$DataTargetDir = Join-Path -Path $OutputDir -ChildPath "$PathDistDESharesModules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\shaders")`
    -TargetDir (Join-Path -Path $DataTargetDir -ChildPath "shaders") -Pattern "*.shader.xml"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\shaderSources")`
    -TargetDir (Join-Path -Path $DataTargetDir -ChildPath "shaderSources") -Pattern "*.glsl"


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy PDBs to '$TargetDir'"

Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "graopengl.pdb") -Destination $TargetDir
