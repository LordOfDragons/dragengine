package ch.dragondreams.delauncher.launcher

import android.graphics.Point

class Game {
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
}