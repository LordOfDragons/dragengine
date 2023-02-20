param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
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
$TargetDir = "$OutputDir\$PathDistIGDESystem"

Write-Host "Igde Shared App: Copy Library to '$TargetDir'"
Install-Files -Path "$OutputDir\deigdeshared.dll" -Destination $TargetDir

$DataTargetDir = "$OutputDir\$PathDistIGDEShares"
Write-Host "Igde Shared App: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir $DataTargetDir -Pattern "*"


# sdk
$TargetDir = "$OutputDir\$PathDistIGDESdkInc\deigde"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "Igde Shared SDK: Copy Headers to '$TargetDir'"
Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"

$TargetDir = "$OutputDir\$PathDistIGDESdkInc\deigde\gui\native"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

$TargetDir = "$OutputDir\$PathDistIGDESdkLib"
Write-Host "Igde Shared SDK: Copy Libraries to '$TargetDir'"

Install-Files -Path "$OutputDir\deigdeshared.lib" -Destination $TargetDir
Install-Files -Path "$OutputDir\deigdeshared.exp" -Destination $TargetDir


# dependencies
Write-Host "Igde Shared: Copy Dependencies to '$TargetDir'"

Copy-Files -SourceDir $FoxDir -TargetDir "$OutputDir\$PathDistIGDEBin" -Pattern "*.dll"


# debug
$TargetDir = "$OutputDir\$PathDistIGDEPdbSystem"
Write-Host "Igde Shared Debug: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\deigdeshared.pdb" -Destination $TargetDir
