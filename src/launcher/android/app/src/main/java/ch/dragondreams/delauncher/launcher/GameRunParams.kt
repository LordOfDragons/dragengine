package ch.dragondreams.delauncher.launcher

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
}