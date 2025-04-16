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
    $TargetDir = "$OutputDir\$PathDistDEDataModules\input\w32input\$Version"
    
    Write-Host "Windows Input Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\input\w32input\inpw32.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
        -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
        -Library $Library -Version $Version
}

Copy-Files -SourceDir "$SourceDir\..\data" -Pattern "*"`
    -TargetDir "$OutputDir\$PathDistDESharesModules\input\w32input\$Version"


# debug
if(!$InternalModule)
{
    $TargetDir = "$OutputDir\$PathDistDEPdbDataModules\input\w32input\$Version"
    Write-Host "Windows Input Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\input\w32input\inpw32.pdb" -Destination $TargetDir
}
