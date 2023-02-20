param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$RuntimeDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$OutputDir\$PathDistDEDataModules\scripting\dragonscript\$Version"

Write-Host "DragonScript Module: Copy Module to '$TargetDir'"

$Library = Join-Path -Path $OutputDir -ChildPath "scrdscript.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
    -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
    -Library $Library -Version $Version


$DataTargetDir = Join-Path -Path $OutputDir -ChildPath "$PathDistDESharesModules\scripting\dragonscript\$Version"
Write-Host "DragonScript Module: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\scripts")`
    -TargetDir (Join-Path -Path $DataTargetDir -ChildPath "scripts") -Pattern "*.ds"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\doc\nativeclasses")`
    -TargetDir (Join-Path -Path $DataTargetDir -ChildPath "native") -Pattern "*.ds"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\data")`
    -TargetDir (Join-Path -Path $DataTargetDir -ChildPath "data") -Pattern "*"

Copy-Files -SourceDir "$RuntimeDir\bin" -Pattern "libdscript.dll" -TargetDir $TargetDir


$RuntimeTargetDir = Join-Path -Path $DataTargetDir -ChildPath "dsinstall"
Write-Host "DragonScript Module: Copy Runtime to '$RuntimeTargetDir'"

Copy-Files -SourceDir "$RuntimeDir\dsinstall" -Pattern "*" -TargetDir $RuntimeTargetDir


# debug
$TargetDir = "$OutputDir\$PathDistDEPdbDataModules\scripting\dragonscript\$Version"
Write-Host "DragonScript Module: Copy PDBs to '$TargetDir'"

Install-Files -Path (Join-Path -Path $OutputDir -ChildPath "scrdscript.pdb") -Destination $TargetDir
Install-Files "$RuntimeDir\pdb" -Destination $TargetDir
