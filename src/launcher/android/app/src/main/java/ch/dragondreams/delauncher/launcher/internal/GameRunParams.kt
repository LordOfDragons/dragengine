package ch.dragondreams.delauncher.launcher.internal

data class GameRunParams(
    var gameProfile: GameProfile? = null,
    var runArguments: String = ""
    //val patches: Array<GamePatch> = emptyArray()
)
