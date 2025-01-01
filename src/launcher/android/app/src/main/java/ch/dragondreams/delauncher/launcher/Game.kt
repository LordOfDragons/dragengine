package ch.dragondreams.delauncher.launcher

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

    /**
     * Custom profile. If you store the object for later use call GameProfile.retain()
     * and GameProfile.release() upon dropping the object.
     *
     * Assigning a non-null profile calls GameProfile.retain() and GameProfile.release()
     * itself.
     */
    var customProfile: GameProfile? = null
        set(value) {
            if(value != field) {
                nativeGame.setCustomProfile(value?.nativeProfile)
                field?.release()
                field = value?.retain()
            }
        }

    /**
     * Active profile. If you store the object for later use call GameProfile.retain()
     *      * and GameProfile.release() upon dropping the object.
     *
     * Assigning a non-null profile calls GameProfile.retain() and GameProfile.release()
     * itself.
     */
    var activeProfile: GameProfile? = null
        set(value) {
            if(value != field) {
                nativeGame.setActiveProfile(value?.nativeProfile)
                field?.release()
                field = value?.retain()
            }
        }

    var runArguments = ""

    var useLatestPatch = false
    var useCustomPatch = ""
    //var localPatches: MutableList<GamePatch> = ArrayList()

    init {
        updateInfo()
        updateConfig()
    }

    fun dispose(){
        activeProfile = null
        customProfile = null

        nativeGame.dispose()
    }

    fun updateInfo(){
        nativeGame.updateInfo(this)
    }

    fun reference(): ch.dragondreams.delauncher.launcher.Game {
        nativeGame.addReference()
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
        customProfile?.updateStatus()
        activeProfile?.updateStatus()
    }

    fun updateConfig() {
        nativeGame.updateConfig(this)
    }

    /** \brief Profile to use. */
    fun getProfileToUse(launcher: DragengineLauncher): GameProfile?{
        return activeProfile
            ?: customProfile
            ?: launcher.activeProfile
            ?: launcher.defaultProfile;
    }
}
