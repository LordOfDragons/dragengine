package ch.dragondreams.delauncher.launcher.internal

data class GameRunParams(
    var gameProfile: Long = 0L,
    var runArguments: String = "",
    var width: Int = 0,
    var height: Int = 0
    //val patches: Array<GamePatch> = emptyArray()
)
