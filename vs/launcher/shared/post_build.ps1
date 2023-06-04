param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

# build
$TargetDir = "$OutputDir\include\delauncher"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "DELauncherShared App: Copy Headers to '$TargetDir'"
Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"


# application
$TargetDir = "$OutputDir\$PathDistDESystem"

Write-Host "DELauncherShared App: Copy Library to '$TargetDir'"
Install-Files -Path "$OutputDir\launcher\shared\delauncher.dll" -Destination $TargetDir

<# enable one data is present otherwise this code fails
$TargetDir = Join-Path -Path $OutputDir -ChildPath $PathDistDELauncherShares

Write-Host "DELauncherShared App: Copy Data to '$TargetDir'"
Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "../shared/data") -TargetDir $TargetDir -Pattern "*"
#>


# sdk
$TargetDir = Join-Path -Path $OutputDir -ChildPath "$PathDistDESdkInc\delauncher"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "DELauncherShared SDK: Copy Headers to '$TargetDir'"
Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"


$TargetDir = Join-Path -Path $OutputDir -ChildPath $PathDistDESdkLib
Write-Host "DELauncherShared SDK: Copy Libraries to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\shared\delauncher.lib" -Destination $TargetDir
Install-Files -Path "$OutputDir\launcher\shared\delauncher.exp" -Destination $TargetDir


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbSystem"
Write-Host "DELauncherShared Debug: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\launcher\shared\delauncher.pdb" -Destination $TargetDir
