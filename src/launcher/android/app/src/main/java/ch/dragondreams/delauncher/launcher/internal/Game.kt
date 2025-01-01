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
    private external fun gameSetConfig(game: Long, config: GameConfig)

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

    fun storeConfig(game: Game){
        var config = GameConfig()
        config.customProfile = game.customProfile?.nativeProfile?.nativeProfile ?: 0L
        config.activeProfile = game.activeProfile?.nativeProfile?.nativeProfile ?: 0L
        gameSetConfig(nativeGame, config)
    }
}
