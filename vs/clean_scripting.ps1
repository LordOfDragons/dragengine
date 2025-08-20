$Directories = @(
    'demodules\%ARCH%',
    'enginedeal\%ARCH%',
    'modules\scripting\dragonscript\%ARCH%'
)

$Architectures = @(
    'x64'
)

ForEach ($Architecture in $Architectures) {
    ForEach ($Directory in $Directories) {
        $DirName = "$Directory".Replace("%ARCH%", $Architecture)
        Write-Host "Remove directory '$DirName'..."
        if (Test-Path $DirName) {
            Remove-Item -Path $DirName -Force -Recurse
        }
    }
}
