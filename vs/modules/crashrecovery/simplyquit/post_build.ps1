param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

# application
$FilePath = Join-Path -Path $SourceDir -ChildPath "..\SConscript"
$Version = & ..\..\..\get_version.ps1 -Path "$FilePath"

$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Dragengine\Application\@ProgramFiles\Dragengine\Data\modules\crashrecovery\simplyquit\$Version"

Write-Host "CRSimplyQuit Module: Copy Module to '$TargetDir'"

$Library = Join-Path -Path $OutputDir -ChildPath "crsimplyquit.dll"
& ..\..\..\install_file.ps1 -Path "$Library" -Destination "$TargetDir"

$ManifestSource = Join-Path -Path $SourceDir -ChildPath "module.xml"
$ManifestTarget = Join-Path -Path $TargetDir -ChildPath "module.xml"
& ..\..\..\copy_manifest.ps1 -Path "$ManifestSource" -Destination "$ManifestTarget" -Library "$Library" -Version "$Version"


# debug
$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Dragengine\Debug\@ProgramFiles\Dragengine\PDB\EngineModules"
Write-Host "CRSimplyQuit Module: Copy PDBs to '$TargetDir'"

$FilePath = Join-Path -Path $OutputDir -ChildPath "crsimplyquit.pdb"
& ..\..\..\install_file.ps1 -Path "$FilePath" -Destination "$TargetDir"
