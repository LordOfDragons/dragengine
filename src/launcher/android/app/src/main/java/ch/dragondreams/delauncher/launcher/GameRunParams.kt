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
    val patches: MutableList<String> = mutableListOf()

    fun toNative(): GameRunParams{
        val params = GameRunParams()
        params.gameProfile = gameProfile?.nativeProfile
        params.runArguments = runArguments
        return params
    }
}
