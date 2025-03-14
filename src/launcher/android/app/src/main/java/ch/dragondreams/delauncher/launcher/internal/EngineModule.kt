package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.EngineModule

data class EngineModule(
    var type: Int = 0, // EngineModule.Type
    var name: String = "",
    var description: String = "",
    var author: String = "",
    var version: String = "",
    var dirName: String = "",
    var pattern: String = "",
    var priority: Int = 0,
    var isFallback: Boolean = false,
    var status: Int = 0, // EngineModule.Status
    var errorCode: Int = 0,
    var libFileName: String = "",
    var libFileSizeShould: Int = 0,
    var libFileSizeIs: Int = 0,
    var libFileHashShould: String = "",
    var libFileHashIs: String = "",
    var libFileEntryPoint: String = "",
    var parameters: Array<EngineModuleParameter> = emptyArray())
{
    fun convert(): EngineModule {
        val module = EngineModule()
        module.type = mapType[type]!!
        module.name = name
        module.description = description
        module.author = author
        module.version = version
        module.dirName = dirName
        module.pattern = pattern
        module.priority = priority
        module.isFallback = isFallback
        module.status = mapStatus[status]!!
        module.errorCode = errorCode
        module.libFileName = libFileName
        module.libFileSizeShould = libFileSizeShould
        module.libFileSizeIs = libFileSizeIs
        module.libFileHashShould = libFileHashShould
        module.libFileHashIs = libFileHashIs
        module.libFileEntryPoint = libFileEntryPoint
        parameters.forEach { p -> module.parameters.add(p.convert()) }
        return module
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as ch.dragondreams.delauncher.launcher.internal.EngineModule

        if (type != other.type) return false
        if (name != other.name) return false
        if (description != other.description) return false
        if (author != other.author) return false
        if (version != other.version) return false
        if (dirName != other.dirName) return false
        if (pattern != other.pattern) return false
        if (priority != other.priority) return false
        if (isFallback != other.isFallback) return false
        if (status != other.status) return false
        if (errorCode != other.errorCode) return false
        if (libFileName != other.libFileName) return false
        if (libFileSizeShould != other.libFileSizeShould) return false
        if (libFileSizeIs != other.libFileSizeIs) return false
        if (libFileHashShould != other.libFileHashShould) return false
        if (libFileHashIs != other.libFileHashIs) return false
        if (libFileEntryPoint != other.libFileEntryPoint) return false
        if (!parameters.contentEquals(other.parameters)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = type
        result = 31 * result + name.hashCode()
        result = 31 * result + description.hashCode()
        result = 31 * result + author.hashCode()
        result = 31 * result + version.hashCode()
        result = 31 * result + dirName.hashCode()
        result = 31 * result + pattern.hashCode()
        result = 31 * result + priority
        result = 31 * result + isFallback.hashCode()
        result = 31 * result + status
        result = 31 * result + errorCode
        result = 31 * result + libFileName.hashCode()
        result = 31 * result + libFileSizeShould
        result = 31 * result + libFileSizeIs
        result = 31 * result + libFileHashShould.hashCode()
        result = 31 * result + libFileHashIs.hashCode()
        result = 31 * result + libFileEntryPoint.hashCode()
        result = 31 * result + parameters.contentHashCode()
        return result
    }

    companion object {
        val mapType: Map<Int, EngineModule.Type> = mapOf(
            0 to EngineModule.Type.Unknown,
            1 to EngineModule.Type.Graphic,
            2 to EngineModule.Type.Audio,
            3 to EngineModule.Type.Input,
            4 to EngineModule.Type.Network,
            5 to EngineModule.Type.Physics,
            6 to EngineModule.Type.Image,
            7 to EngineModule.Type.Video,
            8 to EngineModule.Type.Script,
            9 to EngineModule.Type.Model,
            10 to EngineModule.Type.Rig,
            11 to EngineModule.Type.Skin,
            12 to EngineModule.Type.Animation,
            13 to EngineModule.Type.Font,
            14 to EngineModule.Type.CrashRecovery,
            15 to EngineModule.Type.LanguagePack,
            16 to EngineModule.Type.Animator,
            17 to EngineModule.Type.Sound,
            18 to EngineModule.Type.AI,
            19 to EngineModule.Type.OcclusionMesh,
            20 to EngineModule.Type.Synthesizer,
            21 to EngineModule.Type.Archive,
            22 to EngineModule.Type.VR,
            23 to EngineModule.Type.Service)

        val mapTypeInv: Map<EngineModule.Type, Int> = buildMap {
            mapType.forEach { e -> put(e.value, e.key) }
        }

        val mapStatus: Map<Int, EngineModule.Status> = mapOf(
            0 to EngineModule.Status.Ready,
            1 to EngineModule.Status.NotTested,
            2 to EngineModule.Status.Broken)
    }
}
