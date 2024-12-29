package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.EngineModuleParameterInfo

data class EngineModuleParameterInfoSelectionEntry(
    var value: String = "",
    var displayName: String = "",
    var description: String = "")
{
    fun convert(): EngineModuleParameterInfo.SelectionEntry {
        val entry = EngineModuleParameterInfo.SelectionEntry()
        entry.value = value
        entry.displayName = displayName
        entry.description = description
        return entry
    }
}
