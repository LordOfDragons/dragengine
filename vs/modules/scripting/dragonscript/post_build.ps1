param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$true)][string]$RuntimeDir,
    [Parameter(Mandatory=$false)][switch]$InternalModule = $false,
    [Parameter(Mandatory=$false)][switch]$WithEngineDeal = $false
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

# application
$Version = Get-Version -Path (Join-Path -Path $SourceDir -ChildPath "..\SConscript")

$TargetDir = "$DistributeDir\$PathDistDEDataModules\scripting\dragonscript\$Version"

if(!$InternalModule)
{
    Write-Host "DragonScript Module: Copy Module to '$TargetDir'"
    
    $Library = "$OutputDir\de_module\scripting\dragonscript\scrdscript.dll"
    Install-Files -Path $Library -Destination $TargetDir
    
    Copy-Manifest -Path (Join-Path -Path $SourceDir -ChildPath "module.xml")`
        -Destination (Join-Path -Path $TargetDir -ChildPath "module.xml")`
        -Library $Library -Version $Version
}


if($WithEngineDeal)
{
    $BaseDataTargetDir = "$OutputDir\..\enginedeal\modules\scripting\dragonscript"
    if (Test-Path $BaseDataTargetDir) {
        Remove-Item $BaseDataTargetDir -Force -Recurse
    }
    
    $DataTargetDir = "$BaseDataTargetDir\$Version"
    New-Item -ItemType Directory $DataTargetDir | Out-Null
}
else
{
    $DataTargetDir = "$DistributeDir\$PathDistDESharesModules\scripting\dragonscript\$Version"
}

Write-Host "DragonScript Module: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\scripts")`
    -TargetDir "$DataTargetDir\scripts" -Pattern "*.ds"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\doc\nativeclasses")`
    -TargetDir "$DataTargetDir\native" -Pattern "*.ds"

Copy-Files -SourceDir (Join-Path -Path $SourceDir -ChildPath "..\data")`
    -TargetDir "$DataTargetDir\data" -Pattern "*"


Copy-Files -SourceDir "$RuntimeDir\bin" -Pattern "libdscript.dll" -TargetDir $TargetDir


$DataTargetDir = "$DistributeDir\$PathDistDESharesModules\scripting\dragonscript\$Version"
$RuntimeTargetDir = Join-Path -Path $DataTargetDir -ChildPath "dsinstall"
Write-Host "DragonScript Module: Copy Runtime to '$RuntimeTargetDir'"

Copy-Files -SourceDir "$RuntimeDir\dsinstall" -Pattern "*" -TargetDir $RuntimeTargetDir


# igde
$DataTargetDir = "$DistributeDir\$PathDistIGDEShares"
Write-Host "DragonScript Module: Copy IGDE Data to '$DataTargetDir'"

$PathModuleShared = "%{DE_SHARE_PATH}\modules\scripting\dragonscript\$Version\data"

Copy-Files -SourceDir "$SourceDir\..\igde\gamedefs"`
    -TargetDir "$DataTargetDir\gamedefs" -Pattern "*.degd"`
    -Replace1Key "%{PATH_MODULE_SHARED}" -Replace1Value "$PathModuleShared"

Copy-Files -SourceDir "$SourceDir\..\igde\templates" -TargetDir "$DataTargetDir\templates" -Pattern "*"


# debug
$TargetDir = "$DistributeDir\$PathDistDEPdbDataModules\scripting\dragonscript\$Version"
Write-Host "DragonScript Module: Copy PDBs to '$TargetDir'"

if(!$InternalModule)
{
Install-Files -Path "$OutputDir\de_module\scripting\dragonscript\scrdscript.pdb" -Destination $TargetDir
}
Install-Files "$RuntimeDir\pdb\libdscript.pdb" -Destination $TargetDir
