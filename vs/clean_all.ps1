$Architectures = @(
    'x64'
)

$BaseDirectories = @(
    'modules',
    'shared',
    'igde\editors',
    'launcher\console'
)

$Directories = @(
    '%ARCH%',
    'demodules\%ARCH%',
    'dragengine\%ARCH%',
    'enginedeal\%ARCH%',
    'igde\igde\%ARCH%',
    'igde\shared\%ARCH%',
    'tools\blender\%ARCH%'
)

$BaseDirectories | ForEach-Object {
    $BaseDir = $_
    if (Test-Path $BaseDir) {
        Get-ChildItem -Path $BaseDir -Recurse -Directory | ForEach-Object {
            $FullPath = $_.FullName
            $RelPath = $FullPath.Substring((Get-Location).Path.Length + 1)
            $Directories += "$RelPath\%ARCH%"
        }
    }
}

ForEach ($Architecture in $Architectures) {
    ForEach ($Directory in $Directories) {
        $DirName = "$Directory".Replace("%ARCH%", $Architecture)
        Write-Host "Remove directory '$DirName'..."
        if (Test-Path $DirName) {
            Remove-Item -Path $DirName -Force -Recurse
        }
    }
}

if (Test-Path Distribute) {
    Remove-Item -Path Distribute -Force -Recurse
}
