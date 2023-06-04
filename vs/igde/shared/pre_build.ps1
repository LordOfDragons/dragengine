param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$BuildVersion
)

Import-Module "$PSScriptRoot\..\..\shared.psm1"

Write-Host "IGDE Shared: Init configuration header"

$Content = Get-Content -Raw -Path "deigde_configuration.h"
$Content = $Content -creplace "%BuildVersion%","$BuildVersion"
Set-Content -Path (Join-Path -Path $SourceDir -ChildPath "deigde_configuration.h") -Value $Content
