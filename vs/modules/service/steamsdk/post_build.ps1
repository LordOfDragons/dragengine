param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$SolutionDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\service\steamsdk\$Version"

Write-Host "SteamSDK Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\service\steamsdk\srvsteamsdk.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version

$Library = "$SolutionDir\extern\steamsdk\steamsdk\bin\steam_api64.dll"
Install-Files -Path $Library -Destination $TargetDir


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\service\steamsdk\$Version"
Write-Host "SteamSDK Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\service\steamsdk\srvsteamsdk.pdb" -Destination $TargetDir
