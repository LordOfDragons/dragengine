package ch.dragondreams.delauncher.launcher.internal

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