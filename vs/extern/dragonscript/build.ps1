param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = "$ProjectDir\dragonscript-1.5"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "dragonscript-1.5.tar.xz" -UrlPath "dragonscript"

Expand-TarXz -Path "$ProjectDir\dragonscript-1.5.tar.xz" -Destination $ProjectDir

# Build the downloaded Visual Studio solution in-place.
# Locate MSBuild via vswhere if available, otherwise fall back to msbuild.exe in PATH.
$slnPath = "$ExpandedDir\vs\dragonscript.sln"

function Get-MSBuildPath {
	# try vswhere
	$vswhere = Join-Path -Path ${env:ProgramFiles(x86)} -ChildPath "Microsoft Visual Studio\Installer\vswhere.exe"
	if (Test-Path $vswhere) {
		try {
			$inst = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath 2>$null
			if ($inst) {
				$msbuildCandidate = Join-Path -Path $inst -ChildPath "MSBuild\Current\Bin\MSBuild.exe"
				if (Test-Path $msbuildCandidate) { return $msbuildCandidate }
				$msbuildCandidate = Join-Path -Path $inst -ChildPath "MSBuild\15.0\Bin\MSBuild.exe"
				if (Test-Path $msbuildCandidate) { return $msbuildCandidate }
			}
		} catch { }
	}
	# fallback: rely on msbuild in PATH (e.g. Developer Command Prompt)
	return "msbuild.exe"
}

$msbuild = Get-MSBuildPath

if (-not (Test-Path $slnPath)) {
	Write-Error "Solution file $slnPath not found. Abort."
	throw "Solution file not found: $slnPath"
}

Write-Host "Building solution: $slnPath"
Write-Host "Using MSBuild: $msbuild"

# Build arguments
$configuration = "Release"
$platform = "x64"

# Run MSBuild and check Exitcode
& $msbuild $slnPath /m /t:Build /p:Configuration=$configuration /p:Platform=$platform
if ($LASTEXITCODE -ne 0) {
	Write-Error "MSBuild failed with exit code $LASTEXITCODE"
	throw "MSBuild failed with exit code $LASTEXITCODE"
}
