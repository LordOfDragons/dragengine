package ch.dragondreams.delauncher.launcher

import ch.dragondreams.delauncher.launcher.internal.Game

class Game private constructor(
    val nativeGame: Game
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
     * Assigning a non-null profile calls GameProfile.retain() and GameProfile.release().
     *
     * Call storeConfig() after changing value.
     */
    var customProfile: GameProfile? = null
        set(value) {
            if(value != field) {
                field?.release()
                field = value?.retain()
            }
        }

    /**
     * Active profile. If you store the object for later use call GameProfile.retain()
     *      * and GameProfile.release() upon dropping the object.
     *
     * Assigning a non-null profile calls GameProfile.retain() and GameProfile.release().
     *
     * Call storeConfig() after changing value.
     */
    var activeProfile: GameProfile? = null
        set(value) {
            if(value != field) {
                field?.release()
                field = value?.retain()
            }
        }

    var runArguments = ""

    var useLatestPatch = false
    var useCustomPatch = ""
    //var localPatches: MutableList<GamePatch> = ArrayList()

    var customProperties: MutableMap<String, String> = mutableMapOf()

    private var refcount: Int = 1

    init {
        updateInfo()
        updateConfig()
    }

    private fun dispose(){
        activeProfile = null
        customProfile = null

        dropInstance(nativeGame)
        nativeGame.dispose()
    }

    fun updateInfo(){
        nativeGame.updateInfo(this)
    }

    fun retain(): ch.dragondreams.delauncher.launcher.Game {
        refcount++
        return this
    }

    fun release(){
        refcount--;
        if(refcount == 0){
            dispose()
        }
    }

    fun loadConfig() {
        nativeGame.loadConfig()
    }

    fun storeConfig() {
        nativeGame.storeConfig(this)
    }

    fun setGameDirectory(directory: String){
        nativeGame.setGameDirectory(directory)
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

    /**
     * Load configuration from game definition string.
     */
    fun loadStringConfig(config: String, launcher: DragengineLauncher){
        nativeGame.loadStringConfig(config, launcher.launcher!!)
    }

    fun isRunning():Boolean {
        return nativeGame.isRunning()
    }

    companion object {
        private val mapGames: MutableMap<Game, ch.dragondreams.delauncher.launcher.Game> = mutableMapOf()

        fun getInstance(nativeGame: Game?): ch.dragondreams.delauncher.launcher.Game?{
            var game: ch.dragondreams.delauncher.launcher.Game? = null
            if(nativeGame != null) {
                game = mapGames[nativeGame]
                if (game == null) {
                    game = Game(nativeGame)
                    mapGames[nativeGame] = game
                }
            }
            return game
        }

        private fun dropInstance(nativeGame: Game){
            mapGames.remove(nativeGame)
        }

        const val PROPERTY_OWNER_PACKAGE = "android:ownerPackage"
    }
}
