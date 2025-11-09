$Directories = @(
    'demodules\%ARCH%',
    'enginedeal\%ARCH%',
    'modules\ai\deai\%ARCH%',
    'modules\animation\deanim\%ARCH%',
    'modules\animator\deanimator\%ARCH%',
    'modules\archive\delga\%ARCH%',
    'modules\audio\audnull\%ARCH%',
    'modules\audio\openal\%ARCH%',
    'modules\combined\fbx\animation\%ARCH%',
    'modules\combined\fbx\model\%ARCH%',
    'modules\combined\fbx\rig\%ARCH%',
    'modules\combined\fbx\skin\%ARCH%',
    'modules\crashrecovery\basicrecovery\%ARCH%',
    'modules\crashrecovery\simplyquit\%ARCH%',
    'modules\font\defont\%ARCH%',
    'modules\font\freetype\%ARCH%',
    'modules\graphic\null\%ARCH%',
    'modules\graphic\opengl\%ARCH%',
    'modules\image\ies\%ARCH%',
    'modules\image\jpeg\%ARCH%',
    'modules\image\png\%ARCH%',
    'modules\image\png3d\%ARCH%',
    'modules\image\tga\%ARCH%',
    'modules\image\webp\%ARCH%',
    'modules\image\webp3d\%ARCH%',
    'modules\input\w32input\%ARCH%',
    'modules\langpack\delangpack\%ARCH%',
    'modules\model\demodel\%ARCH%',
    'modules\network\basic\%ARCH%',
    'modules\occlusionmesh\deocclusionmesh\%ARCH%',
    'modules\physics\bullet\%ARCH%',
    'modules\rig\derig\%ARCH%',
    'modules\scripting\dragonscript\%ARCH%',
    'modules\service\eossdk\%ARCH%',
    'modules\service\modio\%ARCH%',
    'modules\service\msgdk\%ARCH%',
    'modules\service\steamsdk\%ARCH%',
    'modules\skin\deskin\%ARCH%',
    'modules\sound\oggvorbis\%ARCH%',
    'modules\synthesizer\desynthesizer\%ARCH%',
    'modules\video\apng\%ARCH%',
    'modules\video\theora\%ARCH%',
    'modules\video\webm\%ARCH%',
    'modules\vr\null\%ARCH%',
    'modules\vr\openxr\%ARCH%',
    'shared\fbx\%ARCH%',
    'shared\vulkan\%ARCH%'
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

