param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$TargetDir
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

Write-Host "Shared Vulkan: Copy Headers to '$TargetDir'"

if (Test-Path $TargetDir) {
    Remove-Item $TargetDir -Force -Recurse
}

Copy-Files -SourceDir $SourceDir -TargetDir $TargetDir -Pattern "*.h"
