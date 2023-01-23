param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

Import-Module "$PSScriptRoot\..\shared.psm1"

# build
$TargetDir = Join-Path -Path $OutputDir -ChildPath "include\dragengine"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "Drag[en]gine: Copy Headers to '$TargetDir'"
Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"


# application
$TargetDir = Join-Path -Path $OutputDir -ChildPath $PathDistDESystem

Write-Host "Drag[en]gine App: Copy Library to '$TargetDir'"
Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "dragengine.dll") -Destination $TargetDir


# sdk
$TargetDir = Join-Path -Path $OutputDir -ChildPath "$PathDistDESdkInc\dragengine"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "Drag[en]gine SDK: Copy Headers to '$TargetDir'"
Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"


$TargetDir = Join-Path -Path $OutputDir -ChildPath $PathDistDESdkLib
Write-Host "Drag[en]gine SDK: Copy Libraries to '$TargetDir'"

Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "dragengine.lib") -Destination $TargetDir
Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "dragengine.exp") -Destination $TargetDir


# debug
$TargetDir = Join-Path -Path $OutputDir -ChildPath $PathDistDEPdb
Write-Host "Drag[en]gine Debug: Copy PDBs to '$TargetDir'"

Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "dragengine.pdb") -Destination $TargetDir
