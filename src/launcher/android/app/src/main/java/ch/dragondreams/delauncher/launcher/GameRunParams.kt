package ch.dragondreams.delauncher.launcher

import ch.dragondreams.delauncher.launcher.internal.GameRunParams

class GameRunParams{
    var gameProfile: GameProfile? = null
        set(value) {
            if(value != field){
                field?.release()
                field = value?.retain()
            }
        }

    var runArguments: String = ""
    var width: Int = 0
    var height: Int = 0
    val patches: MutableList<String> = mutableListOf()

    fun toNative(): GameRunParams{
        val params = GameRunParams()
        params.gameProfile = gameProfile?.nativeProfile?.nativeProfile ?: 0L
        params.runArguments = runArguments
        params.width = width
        params.height = height
        return params
    }
}
