param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$LibVersion = "1.1.1"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "libtheora-$LibVersion.tar.xz" -UrlPath "libtheora"

Expand-TarXz -Path "$ProjectDir\libtheora-$LibVersion.tar.xz" -Destination $ExpandedDir

# build static library using Visual Studio solution file
$ExpandedSrcDir = "$ExpandedDir\libtheora-$LibVersion"
$vcprojPath = "$ExpandedSrcDir\win32\VS2008\libtheora\libtheora_static.vcproj"
$vcxprojPath = "$ExpandedSrcDir\win32\VS2008\libtheora\libtheora_static.vcxproj"
$msbuild = Get-MSBuildPath
$devenv = Get-DevenvPath
$dragengineToolset = Get-VisualStudioToolset
$platformVersion = Get-VisualStudioPlatformVersion
$configuration = "Release"
$platform = "x64"
$runtimeLibrary = "MultiThreadedDLL"

& $devenv $vcprojPath /upgrade

Get-ChildItem -Path $ExpandedSrcDir -Filter *.vcxproj -Recurse | ForEach-Object {
    (Get-Content $_.FullName) `
        -replace '<PlatformToolset>.*</PlatformToolset>', "<PlatformToolset>$dragengineToolset</PlatformToolset>" `
        -replace '<WindowsTargetPlatformVersion>.*</WindowsTargetPlatformVersion>', "<WindowsTargetPlatformVersion>$platformVersion</WindowsTargetPlatformVersion>" `
        -replace '<RuntimeLibrary>.*</RuntimeLibrary>', "<RuntimeLibrary>$runtimeLibrary</RuntimeLibrary>" `
        | Set-Content $_.FullName
}

Write-Host "Building project: $vcxprojPath"
Write-Host "Using MSBuild: $msbuild"

$propsPath = "$ExpandedSrcDir\win32\VS2008\libogg.props"
$propsContent = Get-Content $propsPath -Raw
$propsContent = $propsContent -replace '<AdditionalIncludeDirectories>.*</AdditionalIncludeDirectories>', "<AdditionalIncludeDirectories>..\..\..\..\..\..\libogg\build\install\include</AdditionalIncludeDirectories>"
$propsContent = $propsContent -replace '<AdditionalLibraryDirectories>.*</AdditionalLibraryDirectories>', "<AdditionalLibraryDirectories>..\..\..\..\..\..\libogg\build\install\lib</AdditionalLibraryDirectories>"
Set-Content $propsPath -Value $propsContent

$propsPath = "$ExpandedSrcDir\win32\VS2008\libvorbis.props"
$propsContent = Get-Content $propsPath -Raw
$propsContent = $propsContent -replace '<AdditionalIncludeDirectories>.*</AdditionalIncludeDirectories>', "<AdditionalIncludeDirectories>..\..\..\..\..\..\libvorbis\build\install\include</AdditionalIncludeDirectories>"
$propsContent = $propsContent -replace '<AdditionalLibraryDirectories>.*</AdditionalLibraryDirectories>', "<AdditionalLibraryDirectories>..\..\..\..\..\..\libvorbis\build\install\lib</AdditionalLibraryDirectories>"
Set-Content $propsPath -Value $propsContent

& $msbuild $vcxprojPath /m /t:Build /p:Configuration=$configuration /p:Platform=$platform `
	/p:PlatformToolset="$dragengineToolset" /p:RuntimeLibrary="$runtimeLibrary" `
    /p:WindowsTargetPlatformVersion="$platformVersion"
if ($LASTEXITCODE -ne 0) {
	Write-Error "MSBuild failed with exit code $LASTEXITCODE"
	throw "MSBuild failed with exit code $LASTEXITCODE"
}

# copy the built static library to the expected location
$projectDir = Split-Path -Path $vcxprojPath -Parent
$libBuildDir = "$projectDir\x64\Release"
$libFile = "$libBuildDir\libtheora_static.lib"
$targetLibDir = "$ExpandedDir\install\lib"

if (-not (Test-Path $targetLibDir)) {
	New-Item -Path $targetLibDir -ItemType Directory | Out-Null
}

Copy-Item "$libBuildDir\libtheora_static.lib" -Destination "$targetLibDir\libtheora_static.lib" -Force

# copy the include directory
$sourceIncludeDir = "$ExpandedSrcDir\include"
$targetIncludeDir = "$ExpandedDir\install\include"

if (Test-Path $targetIncludeDir) {
	Remove-Item $targetIncludeDir -Force -Recurse
}
Copy-Item $sourceIncludeDir -Destination $targetIncludeDir -Recurse -Force
