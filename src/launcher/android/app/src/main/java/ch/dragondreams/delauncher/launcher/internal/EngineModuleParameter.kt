package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.EngineModuleParameter

data class EngineModuleParameter(
    var index: Int = 0,
    var info: EngineModuleParameterInfo = EngineModuleParameterInfo(),
    var value: String = "")
{
    fun convert(): EngineModuleParameter {
        val parameter = EngineModuleParameter()
        parameter.index = index
        parameter.value = value
        parameter.info = info.convert()
        return parameter
    }
}