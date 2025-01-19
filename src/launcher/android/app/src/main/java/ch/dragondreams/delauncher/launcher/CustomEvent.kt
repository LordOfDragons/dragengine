package ch.dragondreams.delauncher.launcher

import ch.dragondreams.delauncher.launcher.internal.CustomEvent

class CustomEvent {
    enum class Type{
        BackButton
    }

    var type: Type = Type.BackButton

    fun convert(): CustomEvent {
        val ce = CustomEvent()
        ce.type = mapType[type]!!
        // for future expansion
        return ce
    }

    companion object {
        val mapType: Map<Type, Int> = mutableMapOf(
            Type.BackButton to 0
        )
    }
}