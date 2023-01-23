﻿param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

# application
$FilePath = Join-Path -Path $SourceDir -ChildPath "..\..\SConscript"
$Version = & ..\..\..\..\get_version.ps1 -Path "$FilePath"

$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Dragengine\Application\@ProgramFiles\Dragengine\Data\modules\animation\fbxanim\$Version"

Write-Host "FBXAnim Module: Copy Module to '$TargetDir'"

$Library = Join-Path -Path $OutputDir -ChildPath "animfbx.dll"
& ..\..\..\..\install_file.ps1 -Path "$Library" -Destination "$TargetDir"

$ManifestSource = Join-Path -Path $SourceDir -ChildPath "module.xml"
$ManifestTarget = Join-Path -Path $TargetDir -ChildPath "module.xml"
& ..\..\..\..\copy_manifest.ps1 -Path "$ManifestSource" -Destination "$ManifestTarget" -Library "$Library" -Version "$Version"


# debug
$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Dragengine\Debug\@ProgramFiles\Dragengine\PDB\EngineModules"
Write-Host "FBXAnim Module: Copy PDBs to '$TargetDir'"

$FilePath = Join-Path -Path $OutputDir -ChildPath "animfbx.pdb"
& ..\..\..\..\install_file.ps1 -Path "$FilePath" -Destination "$TargetDir"
