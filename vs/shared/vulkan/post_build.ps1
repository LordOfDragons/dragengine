param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$TargetDir
)

Write-Host "Shared Vulkan: Copy Headers to '$TargetDir'"

if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

& ..\..\copy_files.ps1 -SourceDir "$SourceDir" -TargetDir "$TargetDir" -Pattern "*.h"
