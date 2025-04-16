param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$false)][switch]$InternalModule = $false
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

if(!$InternalModule)
{
    $TargetDir = "$OutputDir\$PathDistDEDataModules\image\webp\$Version"
    
    Write-Host "WEBP Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\image\webp\imgwebp.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
        -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
        -Library $Library -Version $Version
}


# debug
if(!$InternalModule)
{
    $TargetDir = "$OutputDir\$PathDistDEPdbDataModules\image\webp\$Version"
    Write-Host "WEBP Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\image\webp\imgwebp.pdb" -Destination $TargetDir
}
