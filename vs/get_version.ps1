param (
    [Parameter(Mandatory=$true)][string]$Path
)

(Select-String -Path $Path -Pattern "versionString = '([0-9.]+)'").Matches.Groups[1].Value
