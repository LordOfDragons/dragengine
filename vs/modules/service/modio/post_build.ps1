param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$SolutionDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$false)][switch]$InternalModule = $false
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path "$SourceDir\..\SConscript"

if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEDataModules\service\modio\$Version"
    
    Write-Host "Mod.io Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\service\modio\srvmodio.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path "$SourceDir\module.xml" `
        -Destination "$TargetDir\module.xml"`
        -Library $Library -Version $Version
}

$DataTargetDir = "$DistributeDir\$PathDistDESharesModules\service\modio\$Version"
Write-Host "Mod.io Module: Copy Data to '$DataTargetDir'"

# Copy-Files -SourceDir "$SourceDir\..\data" `
#     -TargetDir $DataTargetDir -Pattern "*.*"


# debug
if(!$InternalModule)
{
    $TargetDir = "$DistributeDir\$PathDistDEPdbDataModules\service\modio\$Version"
    Write-Host "Mod.io Module: Copy PDBs to '$TargetDir'"
    
    Install-Files -Path "$OutputDir\de_module\service\modio\srvmodio.pdb" `
        -Destination $TargetDir
}
