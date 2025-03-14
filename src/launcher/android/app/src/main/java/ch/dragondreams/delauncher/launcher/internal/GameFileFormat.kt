package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.GameFileFormat

data class GameFileFormat(
    var type: Int = 0, //EngineModule.Type
    var pattern: String = "",
    var supported: Boolean = false)
{
    fun convert(): GameFileFormat {
        val format = GameFileFormat()
        format.type = EngineModule.mapType[type]!!
        format.pattern = pattern
        format.supported = supported
        return format
    }
}
