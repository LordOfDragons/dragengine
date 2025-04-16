param (
    [Parameter(Mandatory=$true)][string]$SourceDir
)

Import-Module "$PSScriptRoot\..\..\..\..\shared.psm1"

UpdateModuleVersion -SourceDir $SourceDir
