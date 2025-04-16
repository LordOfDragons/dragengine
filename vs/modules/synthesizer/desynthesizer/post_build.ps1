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
    $TargetDir = "$OutputDir\$PathDistDEDataModules\synthesizer\desynthesizer\$Version"
    
    Write-Host "DESynthesizer Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\synthesizer\desynthesizer\synthdesynthesizer.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
        -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
        -Library $Library -Version $Version
}


# debug
if(!$InternalModule)
{
    $TargetDir = "$OutputDir\$PathDistDEPdbDataModules\synthesizer\desynthesizer\$Version"
    Write-Host "DESynthesizer Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\synthesizer\desynthesizer\synthdesynthesizer.pdb" -Destination $TargetDir
}
