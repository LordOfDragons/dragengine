package ch.dragondreams.delauncher.launcher

import ch.dragondreams.delauncher.launcher.internal.Game

class Game(
    private val nativeGame: Game
) {
    private var nativeGameRefCount: Int = 0

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
        if(nativeGameRefCount-- == 0) {
            nativeGame.dispose()
        }
    }

    fun updateInfo(){
        nativeGame.updateInfo(this)
    }

    fun reference(): ch.dragondreams.delauncher.launcher.Game {
        nativeGameRefCount++
        return this
    }

    fun loadConfig() {
        nativeGame.loadConfig()
    }

    fun verifyRequirements() {
        nativeGame.verifyRequirements()
    }

    fun updateStatus() {
        nativeGame.updateStatus(this)
    }
}
