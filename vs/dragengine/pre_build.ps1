param (
    [Parameter(Mandatory=$true)][string]$SourceDir
)

Write-Host "Drag[en]gine: Init configuration header"
Copy-Item -Path "dragengine_configuration.h" -Destination (Join-Path -Path $SourceDir -ChildPath "dragengine_configuration.h") -Force
