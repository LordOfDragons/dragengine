package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.Game

class Game(
    private var nativeGame: Long = 0L
){
    private var nativeRefCount: Int = 1

    private external fun gameRelease(game: Long)
    private external fun gameGetInfo(game: Long): GameInfo
    private external fun gameLoadConfig(game: Long)
    private external fun gameVerifyRequirements(game: Long)
    private external fun gameGetStatus(game: Long): GameStatus
    private external fun gameGetConfig(game: Long): GameConfig
    private external fun gameSetCustomProfile(game: Long, profile: Long)
    private external fun gameSetActiveProfile(game: Long, profile: Long)

    fun dispose() {
        if (nativeRefCount-- == 0 && nativeGame != 0L){
            gameRelease(nativeGame)
            nativeGame = 0L
        }
    }

    fun addReference() {
        nativeRefCount++
    }

    fun updateInfo(game: Game){
        gameGetInfo(nativeGame).update(game)
    }

    fun loadConfig() {
        gameLoadConfig(nativeGame)
    }

    fun verifyRequirements() {
        gameVerifyRequirements(nativeGame)
    }

    fun updateStatus(game: Game) {
        gameGetStatus(nativeGame).update(game)
    }

    fun updateConfig(game: Game) {
        gameGetConfig(nativeGame).update(game)
    }

    fun setCustomProfile(profile: GameProfile?){
        gameSetCustomProfile(nativeGame, profile?.nativeProfile ?: 0L)
    }

    fun setActiveProfile(profile: GameProfile?){
        gameSetActiveProfile(nativeGame, profile?.nativeProfile ?: 0L)
    }
}
