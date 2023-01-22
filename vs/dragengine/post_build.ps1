param (
    [Parameter(Mandatory=$true)][string]$solutiondir
)

$SourceDir = Join-Path -ChildPath ..\src\dragengine\src -Path $solutiondir -Resolve
$TargetDir = Join-Path -ChildPath .\include\dragengine -Path $solutiondir

Write-Host "Drag[en]gine: Copy Headers"

if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

New-Item -ItemType Directory $TargetDir -ErrorAction SilentlyContinue | Out-Null

$CutLength = $SourceDir.Length + 1

Get-ChildItem -Path (Join-Path -Path $SourceDir -ChildPath '*.h') -Recurse | ForEach-Object {
    $RelativePath = $_.FullName.Substring($CutLength)
    $TargetPath = Join-Path -Path $TargetDir -ChildPath $RelativePath
    $ParentPath = Split-Path -Path $TargetPath -Parent
    # Write-Host $RelativePath
    if (!(Test-Path $ParentPath)) {
        New-Item -ItemType Directory $ParentPath -ErrorAction SilentlyContinue | Out-Null
    }
    Copy-Item -Path $_.FullName -Destination (Join-Path -Path $TargetDir -ChildPath $RelativePath)
}
