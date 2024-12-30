package ch.dragondreams.delauncher.launcher

import android.graphics.Point
import ch.dragondreams.delauncher.launcher.internal.Game

class Game(
    private val nativeGame: Game
) {
    var identifier = ""
    var aliasIdentifier = ""
    var title = ""
    var description = ""
    var creator = ""
    var homepage = ""
    val icons: MutableList<GameIcon> = ArrayList()
    var dataDirectory = ""
    var scriptDirectory = ""
    var gameObject = ""
    var scriptModule = ""
    var scriptModuleVersion = ""
    var fileFormats: MutableList<GameFileFormat> = ArrayList()
    var pathConfig = ""
    var pathCapture = ""

    var allFormatsSupported = false
    var scriptModuleFound = false
    var scriptModuleFoundVersion = ""
    var gameUpToDate = false
    var canRun = false

    //var customProfile: GameProfile? = null
    //var activeProfile: GameProfile? = null

    var runArguments = ""

    var useLatestPatch = false
    var useCustomPatch = ""
    //var localPatches: MutableList<GamePatch> = ArrayList()

    init {
        updateInfo()
    }

    fun dispose(){
        nativeGame.dispose()
    }

    fun updateInfo(){
        nativeGame.update(this)
    }

    fun reference(): ch.dragondreams.delauncher.launcher.Game {
        return Game(nativeGame.reference())
    }

    fun loadConfig() {
        nativeGame.loadConfig()
    }

    fun verifyRequirements() {
        nativeGame.verifyRequirements()
    }
}
