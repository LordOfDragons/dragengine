param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$false)][switch]$InternalModule = $false
)

Import-Module "$PSScriptRoot\..\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\..\SConscript")

if(!$InternalModule)
{
    $TargetDir = "$OutputDir\$PathDistDEDataModules\rig\fbxrig\$Version"
    
    Write-Host "FBXRig Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\rig\fbxrig\rigfbx.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
        -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
        -Library $Library -Version $Version
}


# debug
if(!$InternalModule)
{
    $TargetDir = "$OutputDir\$PathDistDEPdbDataModules\rig\fbxrig\$Version"
    Write-Host "FBXRig Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\rig\fbxrig\rigfbx.pdb" -Destination $TargetDir
}
