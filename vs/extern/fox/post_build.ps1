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
$LibVersion = "1.7.85"

# build static library using Visual Studio solution file
$ExpandedSrcDir = "$ExpandedDir\fox-$LibVersion"
$solutionPath = "$ExpandedSrcDir\windows\vs.sln"

# copy the built static library to the expected location
$projectDir = Split-Path -Path $solutionPath -Parent
$libBuildDir = "$ExpandedSrcDir\windows\x64\Release"

$targetLibDir = "$ExpandedDir\install\lib"

if (-not (Test-Path $targetLibDir)) {
	New-Item -Path $targetLibDir -ItemType Directory | Out-Null
}

Copy-Item -Path "$libBuildDir\fox-*.dll" -Destination "$targetLibDir" -Force
Copy-Item -Path "$libBuildDir\fox-*.lib" -Destination "$targetLibDir" -Force
Copy-Item -Path "$libBuildDir\fox-*.pdb" -Destination "$targetLibDir" -Force
Copy-Item -Path "$libBuildDir\fox-*.exp" -Destination "$targetLibDir" -Force

# copy the include directory
$sourceIncludeDir = "$ExpandedSrcDir\include"
$targetIncludeDir = "$ExpandedDir\install\include\fox-1.7"

if (Test-Path $sourceIncludeDir) {
	if (Test-Path $targetIncludeDir) {
		Remove-Item $targetIncludeDir -Force -Recurse
	}
	Copy-Item $sourceIncludeDir -Destination $targetIncludeDir -Recurse -Force
} else {
	throw "Include directory not found at: $sourceIncludeDir"
}
