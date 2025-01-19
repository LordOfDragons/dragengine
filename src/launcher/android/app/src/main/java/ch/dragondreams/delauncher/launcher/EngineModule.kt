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
    }

    enum class ErrorCode{
        Success,
        CreateModuleFailed,
        LibFileNotFound,
        LibFileNotRegularFile,
        LibFileSizeMismatch,
        LibFileCheckSumMismatch,
        LibFileOpenFailed,
        LibFileEntryPointNotFound,
        LibFileCreateModuleFailed
    }

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
    val parameters: MutableList<EngineModuleParameter> = ArrayList()

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

        val mapErrorCode: Map<Int, ErrorCode> = mapOf(
            0 to ErrorCode.Success,
            1 to ErrorCode.CreateModuleFailed,
            10 to ErrorCode.LibFileNotFound,
            11 to ErrorCode.LibFileNotRegularFile,
            12 to ErrorCode.LibFileSizeMismatch,
            13 to ErrorCode.LibFileCheckSumMismatch,
            14 to ErrorCode.LibFileOpenFailed,
            15 to ErrorCode.LibFileEntryPointNotFound,
            16 to ErrorCode.LibFileCreateModuleFailed)

        val mapErrorText: Map<ErrorCode, String> = mapOf(
            ErrorCode.CreateModuleFailed to
                "Creating module failed",
            ErrorCode.LibFileNotFound to
                "Module library file could not be found",
            ErrorCode.LibFileNotRegularFile to
                "Module library file is not a regular library file",
            ErrorCode.LibFileSizeMismatch to
                "Size of the module library file does not match the size in the module definition",
            ErrorCode.LibFileCheckSumMismatch to
                "Checksum of the module library file does not match the checksum in the module definition",
            ErrorCode.LibFileOpenFailed to
                "Module library file could not be opened",
            ErrorCode.LibFileEntryPointNotFound to
                "Entry point function specified in the module definition could not be found in the library file",
            ErrorCode.LibFileCreateModuleFailed to
                "Creating the module using the entry point function failed")
    }
}
