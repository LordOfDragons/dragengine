param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

# build
$TargetDir = Join-Path -Path $OutputDir -ChildPath "include\dragengine"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "Drag[en]gine: Copy Headers to '$TargetDir'"
& ..\copy_files.ps1 -SourceDir "$SourceDir" -TargetDir "$TargetDir" -Pattern "*.h"


# application
$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Application\@System"

Write-Host "Drag[en]gine App: Copy Library to '$TargetDir'"

$FilePath = Join-Path -Path $OutputDir -ChildPath "dragengine.dll"
& ..\install_file.ps1 -Path "$FilePath" -Destination "$TargetDir"


# sdk
$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\SDK\@ProgramFiles\Dragengine\SDK\include\dragengine"
if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Write-Host "Drag[en]gine SDK: Copy Headers to '$TargetDir'"
& ..\copy_files.ps1 -SourceDir "$SourceDir" -TargetDir "$TargetDir" -Pattern "*.h"


$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\SDK\@ProgramFiles\Dragengine\SDK\lib"
Write-Host "Drag[en]gine SDK: Copy Libraries to '$TargetDir'"

$FilePath = Join-Path -Path $OutputDir -ChildPath "dragengine.lib"
& ..\install_file.ps1 -Path "$FilePath" -Destination "$TargetDir"

$FilePath = Join-Path -Path $OutputDir -ChildPath "dragengine.exp"
& ..\install_file.ps1 -Path "$FilePath" -Destination "$TargetDir"


# debug
$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Debug\@ProgramFiles\Dragengine\PDB"
Write-Host "Drag[en]gine Debug: Copy PDBs to '$TargetDir'"

$FilePath = Join-Path -Path $OutputDir -ChildPath "dragengine.pdb"
& ..\install_file.ps1 -Path "$FilePath" -Destination "$TargetDir"
