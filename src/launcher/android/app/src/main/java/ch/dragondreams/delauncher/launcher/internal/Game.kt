package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.Game

class Game(
    private var nativeGame: Long = 0L
){
    private external fun gameRelease(game: Long)
    private external fun gameGetInfo(game: Long): GameInfo
    private external fun gameReference(game: Long): Long
    private external fun gameLoadConfig(game: Long)
    private external fun gameVerifyRequirements(game: Long)

    fun dispose() {
        if (nativeGame != 0L){
            gameRelease(nativeGame)
        }
    }

    fun update(game: Game){
        gameGetInfo(nativeGame).update(game)
    }

    fun reference(): ch.dragondreams.delauncher.launcher.internal.Game{
        return Game(gameReference(nativeGame))
    }

    fun loadConfig() {
        gameLoadConfig(nativeGame)
    }

    fun verifyRequirements() {
        gameVerifyRequirements(nativeGame)
    }
}
