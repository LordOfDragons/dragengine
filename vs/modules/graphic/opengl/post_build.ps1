﻿param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$false)][switch]$InternalModule = $false,
    [Parameter(Mandatory=$false)][switch]$WithEngineDeal = $false
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEDataModules\graphic\opengl\$Version"
    Write-Host "OpenGL Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\graphic\opengl\graopengl.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
        -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
        -Library $Library -Version $Version
}

if($WithEngineDeal)
{
    $BaseDataTargetDir = [IO.Path]::GetFullPath("$OutputDir\..\enginedeal\modules\graphic\opengl")
    if (Test-Path $BaseDataTargetDir) {
        Remove-Item $BaseDataTargetDir -Force -Recurse
    }
    
    $DataTargetDir = "$BaseDataTargetDir\$Version"
    New-Item -ItemType Directory $DataTargetDir | Out-Null
}
else
{
    $DataTargetDir = "$DistributeDir\$PathDistDESharesModules\graphic\opengl\$Version"
}

Write-Host "OpenGL Module: Copy Data to '$DataTargetDir'"
Copy-Files -SourceDir "$SourceDir\..\data" -Pattern "*" -TargetDir $DataTargetDir


# debug
if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEPdbDataModules\graphic\opengl\$Version"
    Write-Host "OpenGL Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\graphic\opengl\graopengl.pdb" -Destination $TargetDir
}
