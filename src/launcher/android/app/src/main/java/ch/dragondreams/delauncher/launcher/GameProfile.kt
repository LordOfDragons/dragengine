package ch.dragondreams.delauncher.launcher

import ch.dragondreams.delauncher.launcher.internal.GameProfile


class GameProfile private constructor(
    val nativeProfile: GameProfile
) {
    class Module{
        var name = ""

        /**
         * Parameters to set. Key is parameter name and value is parameter value.
         */
        val parameters: MutableMap<String, String> = mutableMapOf()
    }

    var name = ""

    var moduleGraphic = ""
    var moduleInput = ""
    var modulePhysics = ""
    var moduleAnimator = ""
    var moduleAI = ""
    var moduleCrashRecovery = ""
    var moduleAudio = ""
    var moduleSynthesizer = ""
    var moduleNetwork = ""
    var moduleVR = ""

    var moduleGraphicVersion = ""
    var moduleInputVersion = ""
    var modulePhysicsVersion = ""
    var moduleAnimatorVersion = ""
    var moduleAIVersion = ""
    var moduleCrashRecoveryVersion = ""
    var moduleAudioVersion = ""
    var moduleSynthesizerVersion = ""
    var moduleNetworkVersion = ""
    var moduleVRVersion = ""

    /**
     * Module versions to disable. Key is name of module and value is version of module.
     */
    val disableModuleVersions: MutableMap<String, String> = mutableMapOf()

    val modules: MutableList<Module> = mutableListOf()

    var runArguments = ""
    var replaceRunArguments = false

    var valid = false

    private var refcount: Int = 1

    init {
        updateInfo()
        updateStatus()
    }

    private fun dispose(){
        dropInstance(nativeProfile)
        nativeProfile.dispose()
    }

    fun updateInfo(){
        nativeProfile.updateInfo(this)
    }

    fun updateStatus(){
        nativeProfile.updateStatus(this)
    }

    fun retain(): ch.dragondreams.delauncher.launcher.GameProfile {
        refcount++
        nativeProfile.addReference()
        return this
    }

    fun release(){
        if(refcount-- == 0){
            dispose()
        }
    }

    companion object {
        private val mapProfiles: MutableMap<GameProfile, ch.dragondreams.delauncher.launcher.GameProfile> = mutableMapOf()

        fun getInstance(nativeProfile: GameProfile?): ch.dragondreams.delauncher.launcher.GameProfile?{
            var profile: ch.dragondreams.delauncher.launcher.GameProfile? = null
            if(nativeProfile != null) {
                profile = mapProfiles[nativeProfile]
                if (profile == null) {
                    profile = GameProfile(nativeProfile)
                    mapProfiles[nativeProfile] = profile
                }
            }
            return profile
        }

        fun createInstance(): ch.dragondreams.delauncher.launcher.GameProfile{
            return getInstance(GameProfile.createInstance())!!
        }

        fun copyInstance(profile: ch.dragondreams.delauncher.launcher.GameProfile):
                ch.dragondreams.delauncher.launcher.GameProfile{
            return getInstance(GameProfile.copyInstance(profile.nativeProfile))!!
        }

        private fun dropInstance(nativeProfile: GameProfile){
            mapProfiles.remove(nativeProfile)
        }
    }
}
