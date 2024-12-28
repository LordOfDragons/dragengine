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

    companion object {
        val mapTypeName: Map<Type, String> = mapOf(
            Type.Unknown to "Unknown",
            Type.Graphic to "Graphic",
            Type.Audio to "Audio",
            Type.Input to "Input",
            Type.Network to "Network",
            Type.Physics to "Physics",
            Type.Image to "Image",
            Type.Video to "Video",
            Type.Script to "Script",
            Type.Model to "Model",
            Type.Rig to "Rig",
            Type.Skin to "Skin",
            Type.Animation to "Animation",
            Type.Font to "Font",
            Type.CrashRecovery to "Crash Recovery",
            Type.LanguagePack to "Language Pack",
            Type.Animator to "Animator",
            Type.Sound to "Sound",
            Type.AI to "AI",
            Type.OcclusionMesh to "Occlusion Mesh",
            Type.Synthesizer to "Synthesizer",
            Type.Archive to "Archive",
            Type.VR to "VR",
            Type.Service to "Service"
        )

        val mapStatusName: Map<Status, String> = mapOf(
            Status.Ready to "Ready",
            Status.NotTested to "Not Tested",
            Status.Broken to "Broken"
        )
    }
}
