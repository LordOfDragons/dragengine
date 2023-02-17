param (
    [Parameter(Mandatory=$true)][string]$SourceDir
)

Import-Module "$PSScriptRoot\..\..\..\shared.psm1"

$ModuleVersion = Get-Version -Path "$SourceDir\..\SConscript"

$Content = Get-Content -Raw -Path "module_version.h"
$Content = $Content -creplace "%ModuleVersion%","$ModuleVersion"
Set-Content -Path (Join-Path -Path $SourceDir -ChildPath "module_version.h") -Value $Content
