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
        New-Item -ItemType Directory $Destination | Out-Null
    }

    if (!$Name) {
        $Name = Split-Path -Path $Path -Leaf
    }

    Copy-Item -Path $Path -Destination (Join-Path -Path $Destination -ChildPath $Name) -Force
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
        [Parameter(Mandatory=$true)][string]$Pattern,
        [Parameter(Mandatory=$false)][string]$Replace1Key,
        [Parameter(Mandatory=$false)][string]$Replace1Value
    )

    $SourceDir = Resolve-Path $SourceDir

    if (!(Test-Path $TargetDir)) {
        New-Item -ItemType Directory $TargetDir | Out-Null
    }

    $CutLength = $SourceDir.Length + 1

    Get-ChildItem -Path (Join-Path -Path $SourceDir -ChildPath $Pattern) -Recurse | ForEach-Object {
        $RelativePath = $_.FullName.Substring($CutLength)
        $TargetPath = Join-Path -Path $TargetDir -ChildPath $RelativePath
        $ParentPath = Split-Path -Path $TargetPath -Parent
        # Write-Host $RelativePath
        if (!(Test-Path $ParentPath)) {
            New-Item -ItemType Directory $ParentPath | Out-Null
        }
        
        if ($Replace1Key) {
            $Content = Get-Content -Raw -Path $_.FullName
            $Content = $Content -creplace "$Replace1Key","$Replace1Value"
            Set-Content -Path "$TargetDir\$RelativePath" -Value $Content

        }else{
            Copy-Item -Path $_.FullName -Destination "$TargetDir\$RelativePath" -Force
        }
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
        [Parameter(Mandatory=$false)][string]$Version = "1.0"
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


# Unpack *.tar.xz
###################

function Expand-TarXz {
    param (
        [Parameter(Mandatory=$true)][string]$Path,
        [Parameter(Mandatory=$true)][string]$Destination
    )

    # we can not use the piping solution since windows sucks so hard at piping
    # the data gets corrupted along the pipe and tar fails
    # & $PSScriptRoot\bin\7z.exe x "$Path" -so -tbzip2 | tar -xf - -C "$Destination"
    $TarXzFile = Split-Path -Path $Path -Leaf
    $TarFile = $TarXzFile.Substring(0, $TarXzFile.Length - 3)
    $UnpackDir = $TarFile.Substring(0, $TarFile.Length - 4)

    if (Test-Path $UnpackDir) {
        Remove-Item $UnpackDir -Force -Recurse
    }

    7z.exe x "$Path" -txz -y
    7z.exe x "$TarFile" -ttar -y -o"$Destination"
    Remove-Item -Path $TarFile -Force
}


# Sanitize script input path
# --------------------------
# Visual Studio has the tendency to forget strip double quote from the end
# of path send to script files causing various failures. This function
# strips trailing double quote.
# 
# Furthermore such path can also contain a trailing backslash where there
# should be none. This function also strips those

function SanitizeScriptInputPath {
    param (
        [Parameter(Mandatory=$true)][string]$Path
    )

    if($Path.EndsWith('"')){
        $Path = $Path.Substring(0, $Path.Length - 1)
    }

    if($Path.EndsWith('\\')){
        $Path = $Path.Substring(0, $Path.Length - 1)
    }

    return $Path
}


# Download artifact if not present
# --------------------------------

function DownloadArtifact {
    param (
        [Parameter(Mandatory=$true)][string]$SourceDir,
        [Parameter(Mandatory=$true)][string]$FilenameArtifact,
        [Parameter(Mandatory=$true)][string]$UrlPath
    )

    if (!(Test-Path "$SourceDir\$FilenameArtifact")) {
        Invoke-WebRequest "$UrlExternArtifacts/$UrlPath/$FilenameArtifact" -OutFile "$SourceDir\$FilenameArtifact"
    }
}


# Update module version
# ---------------------

function UpdateModuleVersion {
    param (
        [Parameter(Mandatory=$true)][string]$SourceDir
    )
    
    if (Test-Path "$SourceDir\..\SConscript") {
        $ModuleVersion = Get-Version -Path "$SourceDir\..\SConscript"
    } elseif (Test-Path "$SourceDir\..\..\SConscript") {
        $ModuleVersion = Get-Version -Path "$SourceDir\..\..\SConscript"
    } else {
        throw "Missing SConscript"
    }
    
    $Content = "#pragma once`r`n" +
        "#define MODULE_VERSION `"$ModuleVersion`"`r`n"
    
    Set-Content -Path "$SourceDir\module_version.h" -Value $Content
}


# Various path constants
##########################

New-Variable -Name UrlExternArtifacts -Scope Global -Option ReadOnly -Force `
    -Value "https://dragondreams.s3.eu-central-1.amazonaws.com/dragengine/extern"



New-Variable -Name PathDistDE -Value "Dragengine\Application" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEBase -Value "$PathDistDE\@ProgramFiles\Dragengine" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEData -Value "$PathDistDEBase\Data" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEDataModules -Value "$PathDistDEData\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEShares -Value "$PathDistDEBase\Share" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESharesModules -Value "$PathDistDEShares\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESystem -Value "$PathDistDE\@System" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDELauncherBin -Value "$PathDistDEBase\Launchers\Bin" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDELauncherShares -Value "$PathDistDEBase\Launchers\Share" -Scope Global -Option ReadOnly -Force

New-Variable -Name PathDistDESdk -Value "Dragengine\SDK" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESdkBase -Value "$PathDistDESdk\@ProgramFiles\Dragengine\SDK" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESdkInc -Value "$PathDistDESdkBase\include" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDESdkLib -Value "$PathDistDESdkBase\lib" -Scope Global -Option ReadOnly -Force

New-Variable -Name PathDistDEDebug -Value "Dragengine\Debug" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEPdbBase -Value "$PathDistDEDebug\@ProgramFiles\Dragengine" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEPdbData -Value "$PathDistDEPdbBase\Data" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEPdbDataModules -Value "$PathDistDEPdbData\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEPdbSystem -Value "$PathDistDEDebug\@System" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistDEPdbLauncherBin -Value "$PathDistDEPdbBase\Launchers\Bin" -Scope Global -Option ReadOnly -Force



New-Variable -Name PathDistIGDE -Value "Igde\Application" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEBase -Value "$PathDistIGDE\@ProgramFiles\DEIGDE" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEData -Value "$PathDistIGDEBase\Data" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEDataModules -Value "$PathDistIGDEData\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEShares -Value "$PathDistIGDEBase\Share" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDESharesModules -Value "$PathDistIGDEShares\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDESystem -Value "$PathDistIGDE\@System" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEBin -Value "$PathDistIGDEBase\Bin" -Scope Global -Option ReadOnly -Force

New-Variable -Name PathDistIGDESdk -Value "Igde\SDK" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDESdkBase -Value "$PathDistIGDESdk\@ProgramFiles\DEIGDE\SDK" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDESdkInc -Value "$PathDistIGDESdkBase\include" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDESdkLib -Value "$PathDistIGDESdkBase\lib" -Scope Global -Option ReadOnly -Force

New-Variable -Name PathDistIGDEDebug -Value "Igde\Debug" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEPdbBase -Value "$PathDistIGDEDebug\@ProgramFiles\DEIGDE" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEPdbData -Value "$PathDistIGDEPdbBase\Data" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEPdbDataModules -Value "$PathDistIGDEPdbData\modules" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEPdbSystem -Value "$PathDistIGDEDebug\@System" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistIGDEPdbBin -Value "$PathDistIGDEPdbBase\Bin" -Scope Global -Option ReadOnly -Force



New-Variable -Name PathDistLive -Value "Live" -Scope Global -Option ReadOnly -Force
New-Variable -Name PathDistLivePdb -Value "$PathDistLive" -Scope Global -Option ReadOnly -Force
