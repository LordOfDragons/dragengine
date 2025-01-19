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

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as GameConfig

        if (customProfile != other.customProfile) return false
        if (activeProfile != other.activeProfile) return false
        if (!customProperties.contentEquals(other.customProperties)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = customProfile.hashCode()
        result = 31 * result + activeProfile.hashCode()
        result = 31 * result + customProperties.contentHashCode()
        return result
    }
}
