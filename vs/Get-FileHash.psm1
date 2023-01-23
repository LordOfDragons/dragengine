function Get-FileHash
{
    [CmdletBinding(DefaultParameterSetName = "Path")]
    param(
        [Parameter(Mandatory=$true, ParameterSetName="Path", Position = 0)]
        [System.String[]]
        $Path,

        [Parameter(Mandatory=$true, ParameterSetName="LiteralPath", ValueFromPipelineByPropertyName = $true)]
        [Alias("PSPath")]
        [System.String[]]
        $LiteralPath,
        
        [Parameter(Mandatory=$true, ParameterSetName="Stream")]
        [System.IO.Stream]
        $InputStream,

        [ValidateSet("SHA1", "SHA256", "SHA384", "SHA512", "MACTripleDES", "MD5", "RIPEMD160")]
        [System.String]
        $Algorithm="SHA256"
    )
    
    begin
    {
        # Construct the strongly-typed crypto object
        $hasher = [System.Security.Cryptography.HashAlgorithm]::Create($Algorithm)
    }
    
    process
    {
        if($PSCmdlet.ParameterSetName -eq "Stream")
        {
            GetStreamHash -InputStream $InputStream -RelatedPath $null -Hasher $hasher
        }
        else
        {
            $pathsToProcess = @()
            if($PSCmdlet.ParameterSetName  -eq "LiteralPath")
            {
                $pathsToProcess += Resolve-Path -LiteralPath $LiteralPath | Foreach-Object { $_.ProviderPath }
            }
            if($PSCmdlet.ParameterSetName -eq "Path")
            {
                $pathsToProcess += Resolve-Path $Path | Foreach-Object { $_.ProviderPath }
            }

            foreach($filePath in $pathsToProcess)
            {
                if(Test-Path -LiteralPath $filePath -PathType Container)
                {
                    continue
                }

                try
                {
                    # Read the file specified in $FilePath as a Byte array
                    [system.io.stream]$stream = [system.io.file]::OpenRead($filePath)
                    GetStreamHash -InputStream $stream  -RelatedPath $filePath -Hasher $hasher
                }
                catch [Exception]
                {
                    $errorMessage = 'FileReadError {0}:{1}' -f $FilePath, $_
                    Write-Error -Message $errorMessage -Category ReadError -ErrorId "FileReadError" -TargetObject $FilePath
                    return
                }
                finally
                {
                    if($stream)
                    {
                        $stream.Close()
                    }
                }                            
            }
        }
    }
}

function GetStreamHash
{
    param(
        [System.IO.Stream]
        $InputStream,

        [System.String]
        $RelatedPath,

        [System.Security.Cryptography.HashAlgorithm]
        $Hasher)

    # Compute file-hash using the crypto object
    [Byte[]] $computedHash = $Hasher.ComputeHash($InputStream)
    [string] $hash = [BitConverter]::ToString($computedHash) -replace '-',''

    if ($RelatedPath -eq $null)
    {
        $retVal = [PSCustomObject] @{
            Algorithm = $Algorithm.ToUpperInvariant()
            Hash = $hash
        }
        $retVal.psobject.TypeNames.Insert(0, "Microsoft.Powershell.Utility.FileHash")
        $retVal
    }
    else
    {
        $retVal = [PSCustomObject] @{
            Algorithm = $Algorithm.ToUpperInvariant()
            Hash = $hash
            Path = $RelatedPath
        }
        $retVal.psobject.TypeNames.Insert(0, "Microsoft.Powershell.Utility.FileHash")
        $retVal

    }
}