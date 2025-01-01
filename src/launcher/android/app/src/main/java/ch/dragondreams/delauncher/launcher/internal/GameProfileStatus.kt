package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.GameProfile

data class GameProfileStatus(
    var valid: Boolean = false
){
    fun update(game: GameProfile) {
        game.valid = valid
    }
}
