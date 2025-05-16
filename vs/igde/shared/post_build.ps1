param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$true)][string]$FoxDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# build
$TargetDir = "$OutputDir\include\deigde"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "Igde Shared: Copy Headers to '$TargetDir'"
Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"

$TargetDir = "$OutputDir\include\deigde\gui\native"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}


# application
$TargetDir = "$DistributeDir\$PathDistIGDESystem"

Write-Host "Igde Shared App: Copy Library to '$TargetDir'"
Install-Files -Path "$OutputDir\deigdeshared\deigdeshared.dll" -Destination $TargetDir

$DataTargetDir = "$DistributeDir\$PathDistIGDEShares"
Write-Host "Igde Shared App: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir $DataTargetDir -Pattern "*"


# sdk
$TargetDir = "$DistributeDir\$PathDistIGDESdkInc\deigde"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "Igde Shared SDK: Copy Headers to '$TargetDir'"
Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"

$TargetDir = "$DistributeDir\$PathDistIGDESdkInc\deigde\gui\native"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

$TargetDir = "$DistributeDir\$PathDistIGDESdkLib"
Write-Host "Igde Shared SDK: Copy Libraries to '$TargetDir'"

Install-Files -Path "$OutputDir\deigdeshared\deigdeshared.lib" -Destination $TargetDir
Install-Files -Path "$OutputDir\deigdeshared\deigdeshared.exp" -Destination $TargetDir


# dependencies
Write-Host "Igde Shared: Copy Dependencies to '$TargetDir'"

Copy-Files -SourceDir $FoxDir -TargetDir "$DistributeDir\$PathDistIGDEBin" -Pattern "*.dll"


# debug
$TargetDir = "$DistributeDir\$PathDistIGDEPdbSystem"
Write-Host "Igde Shared Debug: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\deigdeshared\deigdeshared.pdb" -Destination $TargetDir
