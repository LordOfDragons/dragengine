# Shared Functions Module
###########################


# Install files to directory
##############################

function Install-Files {
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
}


# Get version from SConscript
###############################

function Get-Version {
    param (
        [Parameter(Mandatory=$true)][string]$Path
    )

    (Select-String -Path $Path -Pattern "versionString = '([0-9.]+)'").Matches.Groups[1].Value
}


# Copy files to directory
function Copy-Files {
    param (
        [Parameter(Mandatory=$true)][string]$SourceDir,
        [Parameter(Mandatory=$true)][string]$TargetDir,
        [Parameter(Mandatory=$true)][string]$Pattern
    )

    $SourceDir = Resolve-Path $SourceDir

    if (!(Test-Path $TargetDir)) {
        New-Item -ItemType Directory $TargetDir -ErrorAction SilentlyContinue | Out-Null
    }

    $CutLength = $SourceDir.Length + 1

    Get-ChildItem -Path (Join-Path -Path $SourceDir -ChildPath $Pattern) -Recurse | ForEach-Object {
        $RelativePath = $_.FullName.Substring($CutLength)
        $TargetPath = Join-Path -Path $TargetDir -ChildPath $RelativePath
        $ParentPath = Split-Path -Path $TargetPath -Parent
        # Write-Host $RelativePath
        if (!(Test-Path $ParentPath)) {
            New-Item -ItemType Directory $ParentPath -ErrorAction SilentlyContinue | Out-Null
        }
        Copy-Item -Path $_.FullName -Destination (Join-Path -Path $TargetDir -ChildPath $RelativePath)
    }
}


# Copy module manifest
########################

# GitHub bug: Get-FileHash does not exist. We have to fake it
Import-Module "$PSScriptRoot\Get-FileHash.psm1"

function Copy-Manifest {
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
}


# Unpack *.tar.bz2
####################

function Expand-TarBz2 {
    param (
        [Parameter(Mandatory=$true)][string]$Path,
        [Parameter(Mandatory=$true)][string]$Destination
    )

    # we can not use the piping solution since windows sucks so hard at piping
    # the data gets corrupted along the pipe and tar fails
    # & $PSScriptRoot\bin\7z.exe x "$Path" -so -tbzip2 | tar -xf - -C "$Destination"
    $TarBz2File = Split-Path -Path $Path -Leaf
    $TarFile = $TarBz2File.Substring(0, $TarBz2File.Length - 4)
    $UnpackDir = $TarFile.Substring(0, $TarFile.Length - 4)

    if (Test-Path $UnpackDir) {
        Remove-Item $UnpackDir -Force -Recurse
    }

    & $PSScriptRoot\bin\7z.exe x "$Path" -tbzip2 -y
    & $PSScriptRoot\bin\7z.exe x "$TarFile" -ttar -y -o"$Destination"
    Remove-Item -Path $TarFile -Force
}


# Various path constants
##########################

New-Variable -Name PathDistDE -Value "Distribute\Dragengine\Application" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEData -Value "$PathDistDE\@ProgramFiles\Dragengine\Data" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEDataModules -Value "$PathDistDEData\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEShares -Value "$PathDistDE\@ProgramFiles\Dragengine\Share" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESharesModules -Value "$PathDistDEShares\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESystem -Value "$PathDistDE\@System" -Scope Global -Option ReadOnly -Force

New-Variable -Name PathDistDESdk -Value "Distribute\Dragengine\SDK" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESdkInc -Value "$PathDistDESdk\@ProgramFiles\Dragengine\SDK\include" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESdkLib -Value "$PathDistDESdk\@ProgramFiles\Dragengine\SDK\lib" -Scope Global -Option ReadOnly -Force

New-Variable -Name PathDistDEDebug -Value "Distribute\Dragengine\Debug" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEPdb -Value "$PathDistDEDebug\@ProgramFiles\Dragengine\PDB" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEPdbModules -Value "$PathDistDEPdb\EngineModules" -Scope Global -Option ReadOnly -Force
