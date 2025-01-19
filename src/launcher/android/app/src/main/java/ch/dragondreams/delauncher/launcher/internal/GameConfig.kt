package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.Game

data class GameConfig(
    var customProfile: Long = 0L,
    var activeProfile: Long = 0L,
    var customProperties: Array<GameConfigCustomProperty> = emptyArray()
){
    fun update(game: Game) {
        game.customProfile = ch.dragondreams.delauncher.launcher.GameProfile.getInstance(
            GameProfile.getInstance(customProfile))

        game.activeProfile = ch.dragondreams.delauncher.launcher.GameProfile.getInstance(
            GameProfile.getInstance(activeProfile))

        game.customProperties = buildMap {
            customProperties.forEach {
                p -> put(p.key, p.value)
            }
        }.toMutableMap()
    }
}
