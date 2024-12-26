package ch.dragondreams.delauncher.launcher

class EngineModule {
    enum class Type{
        Unknown,
        Graphic,
        Audio,
        Input,
        Network,
        Physics,
        Image,
        Video,
        Script,
        Model,
        Rig,
        Skin,
        Animation,
        Font,
        CrashRecovery,
        LanguagePack,
        Animator,
        Sound,
        AI,
        OcclusionMesh,
        Synthesizer,
        Archive,
        VR,
        Service
    }

    enum class Status{
        Ready,
        NotTested,
        Broken
    };

    var type = Type.Unknown
    var name = ""
    var description = ""
    var author = ""
    var version = ""
    var dirName = ""
    var pattern = ""
    var priority = 0
    var isFallback = false
    var status = Status.NotTested
    var errorCode = 0
    var libFileName = ""
    var libFileSizeShould = 0
    var libFileSizeIs = 0
    var libFileHashShould = ""
    var libFileHashIs = ""
    var libFileEntryPoint = ""
    var parameters: Array<EngineModuleParameter> = emptyArray()
}
