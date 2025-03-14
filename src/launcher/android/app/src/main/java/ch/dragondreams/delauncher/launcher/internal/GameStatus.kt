package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.Game

data class GameStatus(
    var allFormatsSupported: Boolean = false,
    var scriptModuleFound: Boolean = false,
    var scriptModuleFoundVersion: String = "",
    var gameUpToDate: Boolean = false,
    var canRun: Boolean = false
){
    fun update(game: Game) {
        game.allFormatsSupported = allFormatsSupported
        game.scriptModuleFound = scriptModuleFound
        game.scriptModuleFoundVersion = scriptModuleFoundVersion
        game.gameUpToDate = gameUpToDate
        game.canRun = canRun
    }
}
