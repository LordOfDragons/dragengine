param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$false)][switch]$InternalModule = $false
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEDataModules\image\tga\$Version"
    
    Write-Host "TGA Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\image\tga\imgtga.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
        -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
        -Library $Library -Version $Version
}


# debug
if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEPdbDataModules\image\tga\$Version"
    Write-Host "TGA Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\image\tga\imgtga.pdb" -Destination $TargetDir
}
