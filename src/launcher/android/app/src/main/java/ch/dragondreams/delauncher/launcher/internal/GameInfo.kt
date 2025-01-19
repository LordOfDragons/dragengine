package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.Game

data class GameInfo(
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
    fun update(game: Game) {
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
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as GameInfo

        if (identifier != other.identifier) return false
        if (aliasIdentifier != other.aliasIdentifier) return false
        if (title != other.title) return false
        if (description != other.description) return false
        if (creator != other.creator) return false
        if (homepage != other.homepage) return false
        if (!icons.contentEquals(other.icons)) return false
        if (dataDirectory != other.dataDirectory) return false
        if (scriptDirectory != other.scriptDirectory) return false
        if (gameObject != other.gameObject) return false
        if (scriptModule != other.scriptModule) return false
        if (scriptModuleVersion != other.scriptModuleVersion) return false
        if (!fileFormats.contentEquals(other.fileFormats)) return false
        if (pathConfig != other.pathConfig) return false
        if (pathCapture != other.pathCapture) return false
        if (runArguments != other.runArguments) return false

        return true
    }

    override fun hashCode(): Int {
        var result = identifier.hashCode()
        result = 31 * result + aliasIdentifier.hashCode()
        result = 31 * result + title.hashCode()
        result = 31 * result + description.hashCode()
        result = 31 * result + creator.hashCode()
        result = 31 * result + homepage.hashCode()
        result = 31 * result + icons.contentHashCode()
        result = 31 * result + dataDirectory.hashCode()
        result = 31 * result + scriptDirectory.hashCode()
        result = 31 * result + gameObject.hashCode()
        result = 31 * result + scriptModule.hashCode()
        result = 31 * result + scriptModuleVersion.hashCode()
        result = 31 * result + fileFormats.contentHashCode()
        result = 31 * result + pathConfig.hashCode()
        result = 31 * result + pathCapture.hashCode()
        result = 31 * result + runArguments.hashCode()
        return result
    }
}
