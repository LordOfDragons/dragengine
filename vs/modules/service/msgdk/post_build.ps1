param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\service\msgdk\$Version"

Write-Host "MicrosoftGDK Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\service\msgdk\srvmsgdk.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version

$DataTargetDir = "$OutputDir\$PathDistDESharesModules\service\msgdk\$Version"
Write-Host "MicrosoftGDK Module: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir $DataTargetDir -Pattern "*.*"


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\service\msgdk\$Version"
Write-Host "MicrosoftGDK Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\service\msgdk\srvmsgdk.pdb"`
    -Destination $TargetDir
