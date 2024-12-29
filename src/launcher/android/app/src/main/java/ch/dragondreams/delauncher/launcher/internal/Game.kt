package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.Game

data class Game(
    var identifier: String = "",
    var aliasIdentifier: String = "",
    var title: String = "",
    var description: String = "",
    var creator: String = "",
    var homepage: String = "",
    var icons: Array<GameIcon> = emptyArray(),
    var dataDirectory: String = "",
    var scriptDirectory: String = "",
    var gameObject: String = "",
    var scriptModule: String = "",
    var scriptModuleVersion: String = "",
    var fileFormats: Array<GameFileFormat> = emptyArray(),
    var pathConfig: String = "",
    var pathCapture: String = "",
    var runArguments: String = "")
{
    fun convert(): Game {
        val game = Game()
        game.identifier = identifier
        game.aliasIdentifier = aliasIdentifier
        game.title = title
        game.description = description
        game.creator = creator
        game.homepage = homepage
        icons.forEach { i -> game.icons.add(i.convert()) }
        game.dataDirectory = dataDirectory
        game.scriptDirectory = scriptDirectory
        game.gameObject = gameObject
        game.scriptModule = scriptModule
        game.scriptModuleVersion = scriptModuleVersion
        fileFormats.forEach { f -> game.fileFormats.add(f.convert()) }
        game.pathConfig = pathConfig
        game.pathCapture = pathCapture
        game.runArguments = runArguments
        return game
    }
}
