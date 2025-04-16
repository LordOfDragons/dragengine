param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir,
    [Parameter(Mandatory=$true)][string]$GrdkDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

$SourceDir = SanitizeScriptInputPath($SourceDir)
$OutputDir = SanitizeScriptInputPath($OutputDir)
$GrdkDir = SanitizeScriptInputPath($GrdkDir)

# application
$Version = "1.23"

$TargetDir = "$DistributeDir\$PathDistDEDataModules\service\msgdk\$Version"

Write-Host "MicrosoftGDK Module: Copy Module to '$TargetDir'"

$Library = "$OutputDir\de_module\service\msgdk\srvmsgdk.dll"
Install-Files -Path $Library -Destination $TargetDir

Copy-Manifest -Path "$SourceDir\module.xml" -Destination "$TargetDir\module.xml" `
    -Library $Library -Version $Version

$DataTargetDir = "$DistributeDir\$PathDistDESharesModules\service\msgdk\$Version"
Write-Host "MicrosoftGDK Module: Copy Data to '$DataTargetDir'"

Copy-Files -SourceDir "$SourceDir\..\data" -TargetDir $DataTargetDir -Pattern "*.*"

Write-Host "MicrosoftGDK Module: Copy Runtime Libraries to '$TargetDir'"

$CopySrcPath = Join-Path -Path $GrdkDir -ChildPath "ExtensionLibraries"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Xbox.Services.API.C"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "DesignTime"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "CommonConfiguration"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Neutral"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Lib"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Release"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Microsoft.Xbox.Services.GDK.C.Thunks.dll"
Copy-Item -Path $CopySrcPath -Destination $TargetDir

$CopySrcPath = Join-Path -Path $GrdkDir -ChildPath "ExtensionLibraries"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Xbox.XCurl.API"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Redist"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "CommonConfiguration"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "neutral"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "XCurl.dll"
Copy-Item -Path $CopySrcPath -Destination $TargetDir

$CopySrcPath = Join-Path -Path $GrdkDir -ChildPath "ExtensionLibraries"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Xbox.LibHttpClient"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "Redist"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "CommonConfiguration"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "neutral"
$CopySrcPath = Join-Path -Path $CopySrcPath -ChildPath "libHttpClient.GDK.dll"
Copy-Item -Path $CopySrcPath -Destination $TargetDir

# debug
$TargetDir = "$DistributeDir\$PathDistDEPdbDataModules\service\msgdk\$Version"
Write-Host "MicrosoftGDK Module: Copy PDBs to '$TargetDir'"

Install-Files -Path "$OutputDir\de_module\service\msgdk\srvmsgdk.pdb"`
    -Destination $TargetDir
