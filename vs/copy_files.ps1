param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$TargetDir,
    [Parameter(Mandatory=$true)][string]$Pattern
)

$SourceDir = Resolve-Path $SourceDir

if (!(Test-Path $TargetDir)) {
    New-Item -ItemType Directory $TargetDir -ErrorAction SilentlyContinue | Out-Null
}

$CutLength = $SourceDir.Length + 1

Get-ChildItem -Path (Join-Path -Path $SourceDir -ChildPath $Pattern) -Recurse | ForEach-Object {
    $RelativePath = $_.FullName.Substring($CutLength)
    $TargetPath = Join-Path -Path $TargetDir -ChildPath $RelativePath
    $ParentPath = Split-Path -Path $TargetPath -Parent
    # Write-Host $RelativePath
    if (!(Test-Path $ParentPath)) {
        New-Item -ItemType Directory $ParentPath -ErrorAction SilentlyContinue | Out-Null
    }
    Copy-Item -Path $_.FullName -Destination (Join-Path -Path $TargetDir -ChildPath $RelativePath)
}
