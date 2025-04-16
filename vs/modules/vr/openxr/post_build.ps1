﻿param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$false)][switch]$InternalModule = $false
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path "$SourceDir\..\SConscript"

if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEDataModules\vr\openxr\$Version"
    
    Write-Host "OpenXR Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\vr\openxr\vropenxr.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml" -Library $Library -Version $Version
}


#$DataTargetDir = "$DistributeDir\$PathDistDESharesModules\vr\openxr\$Version"
#Write-Host "OpenXR Module: Copy Data to '$DataTargetDir'"

#Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir "$DataTargetDir\data" -Pattern "*"


# debug
if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEPdbDataModules\vr\openxr\$Version"
    Write-Host "OpenXR Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\vr\openxr\vropenxr.pdb" -Destination $TargetDir
}
