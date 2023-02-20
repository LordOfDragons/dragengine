param (
    [Parameter(Mandatory=$true)][string]$SourceDir,
    [Parameter(Mandatory=$true)][string]$BuildVersion
)

Import-Module "$PSScriptRoot\..\shared.psm1"

Write-Host "Drag[en]gine: Init configuration header"

$Content = Get-Content -Raw -Path "dragengine_configuration.h"
$Content = $Content -creplace "%BuildVersion%","$BuildVersion"
Set-Content -Path (Join-Path -Path $SourceDir -ChildPath "dragengine_configuration.h") -Value $Content
