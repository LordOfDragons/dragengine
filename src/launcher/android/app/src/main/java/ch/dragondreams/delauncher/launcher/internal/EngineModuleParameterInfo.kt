package ch.dragondreams.delauncher.launcher.internal

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