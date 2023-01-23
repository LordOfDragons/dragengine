param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir
)

# application
$FilePath = Join-Path -Path $SourceDir -ChildPath "..\SConscript"
$Version = & ..\..\..\get_version.ps1 -Path "$FilePath"

$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Dragengine\Application\@ProgramFiles\Dragengine\Data\modules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy Module to '$TargetDir'"

$Library = Join-Path -Path $OutputDir -ChildPath "graopengl.dll"
& ..\..\..\install_file.ps1 -Path "$Library" -Destination "$TargetDir"

$ManifestSource = Join-Path -Path $SourceDir -ChildPath "module.xml"
$ManifestTarget = Join-Path -Path $TargetDir -ChildPath "module.xml"
& ..\..\..\copy_manifest.ps1 -Path "$ManifestSource" -Destination "$ManifestTarget" -Library "$Library" -Version "$Version"


$DataTargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Dragengine\Application\@ProgramFiles\Dragengine\Share\modules\graphic\opengl\$Version"
Write-Host "OpenGL Module: Copy Data to '$DataTargetDir'"

$DataSourceDir = Join-Path -Path $SourceDir -ChildPath "..\shaders"
$DataTargetDir2 = Join-Path -Path $DataTargetDir -ChildPath "shaders"
& ..\..\..\copy_files.ps1 -SourceDir "$DataSourceDir" -TargetDir "$DataTargetDir2" -Pattern "*.shader.xml"

$DataSourceDir = Join-Path -Path $SourceDir -ChildPath "..\shaderSources"
$DataTargetDir2 = Join-Path -Path $DataTargetDir -ChildPath "shaderSources"
& ..\..\..\copy_files.ps1 -SourceDir "$DataSourceDir" -TargetDir "$DataTargetDir2" -Pattern "*.glsl"


# debug
$TargetDir = Join-Path -Path $OutputDir -ChildPath "Distribute\Dragengine\Debug\@ProgramFiles\Dragengine\PDB\EngineModules"
Write-Host "OpenGL Module: Copy PDBs to '$TargetDir'"

$FilePath = Join-Path -Path $OutputDir -ChildPath "graopengl.pdb"
& ..\..\..\install_file.ps1 -Path "$FilePath" -Destination "$TargetDir"
