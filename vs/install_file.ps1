param (
    [Parameter(Mandatory=$true)][string]$Path,
    [Parameter(Mandatory=$true)][string]$Destination,
    [Parameter(Mandatory=$false)][string]$Name
)

$Path = Resolve-Path $Path

if (!(Test-Path $Destination)) {
    New-Item -ItemType Directory $Destination -ErrorAction SilentlyContinue | Out-Null
}

if (!$Name) {
    $Name = Split-Path -Path $Path -Leaf
}

Copy-Item -Path $Path -Destination (Join-Path -Path $Destination -ChildPath $Name)
