package ch.dragondreams.delauncher.launcher.internal

data class GameProfileModule(
    var name: String = "",
    var parameters: Array<GameProfileModuleParameter> = emptyArray()
)
