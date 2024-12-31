package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.Game

class Game(
    private var nativeGame: Long = 0L
){
    private external fun gameRelease(game: Long)
    private external fun gameGetInfo(game: Long): GameInfo
    private external fun gameLoadConfig(game: Long)
    private external fun gameVerifyRequirements(game: Long)
    private external fun gameGetStatus(game: Long): GameStatus

    fun dispose() {
        if (nativeGame != 0L){
            gameRelease(nativeGame)
            nativeGame = 0L
        }
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
}
