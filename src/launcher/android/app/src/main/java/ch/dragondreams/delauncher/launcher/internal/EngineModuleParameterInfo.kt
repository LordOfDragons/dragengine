package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.EngineModuleParameterInfo

data class EngineModuleParameterInfo(
    var type: Int = 0, // EngineModuleParameter.Type
    var name: String = "",
    var description: String = "",
    var minValue: Float = 0.0F,
    var maxValue: Float = 0.0F,
    var valueStepSize: Float = 0.0F,
    var selectionEntries: Array<EngineModuleParameterInfoSelectionEntry> = emptyArray(),
    var category: Int = 0, // EngineModuleParameter.Category
    var displayName: String = "",
    var defaultValue: String = "")
{
    fun convert(): EngineModuleParameterInfo {
        val info = EngineModuleParameterInfo()
        info.type = mapType[type]!!
        info.name = name
        info.description = description
        info.minValue = minValue.toDouble()
        info.maxValue = maxValue.toDouble()
        info.valueStepSize = valueStepSize.toDouble()
        selectionEntries.forEach { e -> info.selectionEntries.add(e.convert()) }
        info.category = mapCategory[category]!!
        info.displayName = displayName
        info.defaultValue = defaultValue
        return info
    }

    companion object {
        val mapType: Map<Int, EngineModuleParameterInfo.Type> = mapOf(
            0 to EngineModuleParameterInfo.Type.Boolean,
            1 to EngineModuleParameterInfo.Type.Numeric,
            2 to EngineModuleParameterInfo.Type.Ranged,
            3 to EngineModuleParameterInfo.Type.String,
            4 to EngineModuleParameterInfo.Type.Selection)

        val mapCategory: Map<Int, EngineModuleParameterInfo.Category> = mapOf(
            0 to EngineModuleParameterInfo.Category.Basic,
            1 to EngineModuleParameterInfo.Category.Advanced,
            2 to EngineModuleParameterInfo.Category.Expert)
    }
}