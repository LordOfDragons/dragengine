param (
    [Parameter(Mandatory=$true)][string]$ProjectDir,
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [Parameter(Mandatory=$true)][string]$DistributeDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"


$ExpandedDir = Join-Path -Path $ProjectDir -ChildPath "dragonscript-1.5-vc64"
if (Test-Path $ExpandedDir) {
    Remove-Item $ExpandedDir -Force -Recurse
}

DownloadArtifact -SourceDir $ProjectDir -FilenameArtifact "dragonscript-1.5-vc64.zip" -UrlPath "dragonscript"

Expand-Archive -Path "$ProjectDir\dragonscript-1.5-vc64.zip" -DestinationPath $ProjectDir

# visual studio expects *.dll, *.lib and *.pdb to be located in the same directory
# the archive contains though the *.dll in the "bin" directory while the other
# two files are located in the "lib". this is the correct layout for general
# distribution and usage but visual studio freaks out about it. to solve this copy
# the *.dll file to the "lib" directory

Copy-Files -SourceDir (Join-Path -Path $ExpandedDir -ChildPath "bin")`
    -TargetDir (Join-Path -Path $ExpandedDir -ChildPath "lib") -Pattern "*.dll"

Copy-Files -SourceDir (Join-Path -Path $ExpandedDir -ChildPath "pdb")`
    -TargetDir (Join-Path -Path $ExpandedDir -ChildPath "lib") -Pattern "*.pdb"
