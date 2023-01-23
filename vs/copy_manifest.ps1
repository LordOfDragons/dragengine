param (
    [Parameter(Mandatory=$true)][string]$Path,
    [Parameter(Mandatory=$true)][string]$Destination,
    [Parameter(Mandatory=$true)][string]$Library,
    [Parameter(Mandatory=$true)][string]$Version
)

$ItemLibrary = Get-Item -Path $Library
$LibrarySize = $ItemLibrary.Length
$LibraryName = $ItemLibrary.Name
$LibraryHash = (Get-FileHash -Path $Library -Algorithm SHA1).Hash

$Content = Get-Content -Raw -Path $Path
$Content = $Content -creplace "FILENAME","$LibraryName"
$Content = $Content -creplace "FILESIZE","$LibrarySize"
$Content = $Content -creplace "SHA1SUM","$LibraryHash"
$Content = $Content -creplace "VERSION","$Version"
Set-Content -Path $Destination -Value $Content
