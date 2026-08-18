param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


if (Test-Path "$PSScriptRoot\..\..\github_cached_externals") {
    return
}


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "build"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

$LibVersion = "1.7.85"
$DllVersion = "1.7"

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "fox-$LibVersion.tar.xz" -UrlPath "fox"

Expand-TarXz -Path "$ProjectDir\fox-$LibVersion.tar.xz" -Destination $ExpandedDir

# build static library using Visual Studio solution file
$ExpandedSrcDir = "$ExpandedDir\fox-$LibVersion"
$solutionPath = "$ExpandedSrcDir\windows\vs.sln"
$msbuild = Get-MSBuildPath
$dragengineToolset = Get-VisualStudioToolset
$platformVersion = Get-VisualStudioPlatformVersion
$configuration = "Release"
$platform = "x64"
$runtimeLibrary = "MultiThreadedDLL"

Get-ChildItem -Path $ExpandedSrcDir -Filter *.vcxproj -Recurse | ForEach-Object {
    (Get-Content $_.FullName) `
        -replace '<PlatformToolset>.*</PlatformToolset>', "<PlatformToolset>$dragengineToolset</PlatformToolset>" `
        -replace '<WindowsTargetPlatformVersion>.*</WindowsTargetPlatformVersion>', "<WindowsTargetPlatformVersion>$platformVersion</WindowsTargetPlatformVersion>" `
        -replace '<RuntimeLibrary>.*</RuntimeLibrary>', "<RuntimeLibrary>$runtimeLibrary</RuntimeLibrary>" `
        | Set-Content $_.FullName
}

(Get-Content "$ExpandedSrcDir\windows\foxdll\foxdll.vcxproj") `
    -replace '<ConfigurationType>DynamicLibrary</ConfigurationType>', @"
        <ConfigurationType>DynamicLibrary</ConfigurationType>
        <TargetName>fox-$DllVersion</TargetName>
"@ `
    -replace ';HAVE_GLU_H;%\(', ';HAVE_GL_H;HAVE_PNG_H;HAVE_ZLIB_H;%(' `
    -replace '<PrecompiledHeaderFile />', @'
        <PrecompiledHeaderFile />
        <AdditionalIncludeDirectories>..\..\..\..\..\libpng\build\install\include;..\..\..\..\..\zlib\build\install\include</AdditionalIncludeDirectories>
'@ `
    -replace '<OptimizeReferences>true</OptimizeReferences>', @'
        <OptimizeReferences>true</OptimizeReferences>
        <AdditionalLibraryDirectories>..\..\..\..\..\libpng\build\install\lib;..\..\..\..\..\zlib\build\install\lib</AdditionalLibraryDirectories>
'@ `
    -replace ';glu32.lib;%\(', ';glu32.lib;libpng16_static.lib;zlibstatic.lib;%(' `
    | Set-Content "$ExpandedSrcDir\windows\foxdll\foxdll.vcxproj"

Write-Host "Building project: $solutionPath"
Write-Host "Using MSBuild: $msbuild"

& $msbuild $solutionPath /m /t:foxdll /p:Configuration=$configuration /p:Platform=$platform `
	/p:PlatformToolset="$dragengineToolset" /p:RuntimeLibrary="$runtimeLibrary" `
    /p:WindowsTargetPlatformVersion="$platformVersion"
if ($LASTEXITCODE -ne 0) {
	Write-Error "MSBuild failed with exit code $LASTEXITCODE"
	throw "MSBuild failed with exit code $LASTEXITCODE"
}

# we can not copy here the file or MSBuild trashed the files. we have to delay this until the post build phase
