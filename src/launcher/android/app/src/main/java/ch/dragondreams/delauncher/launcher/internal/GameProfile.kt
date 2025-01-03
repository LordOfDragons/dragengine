package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.GameProfile

class GameProfile private constructor(
    val nativeProfile: Long = 0L
){
    private var nativeRefCount: Int = 1

    private external fun gameProfileRelease(game: Long)
    private external fun gameProfileGetConfig(game: Long): GameProfileConfig
    private external fun gameProfileSetConfig(game: Long, config: GameProfileConfig)
    private external fun gameProfileGetStatus(game: Long): GameProfileStatus

    fun dispose() {
        nativeRefCount--;
        if (nativeRefCount == 0){
            gameProfileRelease(nativeProfile)
            dropInstance(nativeProfile)
        }
    }

    fun updateConfig(profile: GameProfile){
        gameProfileGetConfig(nativeProfile).update(profile)
    }

    fun storeConfig(profile: GameProfile){
        val config = GameProfileConfig()
        config.name = profile.name

        config.moduleGraphic = profile.moduleGraphic
        config.moduleInput = profile.moduleInput
        config.modulePhysics = profile.modulePhysics
        config.moduleAnimator = profile.moduleAnimator
        config.moduleAI = profile.moduleAI
        config.moduleCrashRecovery = profile.moduleCrashRecovery
        config.moduleAudio = profile.moduleAudio
        config.moduleSynthesizer = profile.moduleSynthesizer
        config.moduleNetwork = profile.moduleNetwork
        config.moduleVR = profile.moduleVR

        config.moduleGraphicVersion = profile.moduleGraphicVersion
        config.moduleInputVersion = profile.moduleInputVersion
        config.modulePhysicsVersion = profile.modulePhysicsVersion
        config.moduleAnimatorVersion = profile.moduleAnimatorVersion
        config.moduleAIVersion = profile.moduleAIVersion
        config.moduleCrashRecoveryVersion = profile.moduleCrashRecoveryVersion
        config.moduleAudioVersion = profile.moduleAudioVersion
        config.moduleSynthesizerVersion = profile.moduleSynthesizerVersion
        config.moduleNetworkVersion = profile.moduleNetworkVersion
        config.moduleVRVersion = profile.moduleVRVersion

        config.disableModuleVersions = buildList<GameProfileModuleVersion> {
            profile.disableModuleVersions.forEach { e ->
                val version = GameProfileModuleVersion()
                version.name = e.key
                version.version = e.value
                add(version)
            }
        }.toTypedArray()

        config.modules = buildList<GameProfileModule> {
            profile.modules.forEach { m ->
                val module = GameProfileModule()
                module.name = m.name
                module.parameters = buildList<GameProfileModuleParameter> {
                    m.parameters.forEach { p ->
                        val param = GameProfileModuleParameter()
                        param.name = p.key
                        param.value = p.value
                        add(param)
                    }
                }.toTypedArray()
                add(module)
            }
        }.toTypedArray()

        config.runArgs = profile.runArguments
        config.replaceRunArgs = profile.replaceRunArguments

        gameProfileSetConfig(nativeProfile, config)
    }

    fun updateStatus(profile: GameProfile){
        gameProfileGetStatus(nativeProfile).update(profile)
    }

    companion object {
        private external fun gameProfileCreate(): Long
        private external fun gameProfileCopy(profile: Long): Long

        private val mapProfiles: MutableMap<Long, ch.dragondreams.delauncher.launcher.internal.GameProfile> = mutableMapOf()

        fun getInstance(nativeProfile: Long): ch.dragondreams.delauncher.launcher.internal.GameProfile?{
            var profile: ch.dragondreams.delauncher.launcher.internal.GameProfile? = null
            if(nativeProfile != 0L) {
                profile = mapProfiles[nativeProfile]
                if (profile != null) {
                    profile.nativeRefCount++
                }else{
                    profile = GameProfile(nativeProfile)
                    mapProfiles[nativeProfile] = profile
                }
            }
            return profile
        }

        fun createInstance(): ch.dragondreams.delauncher.launcher.internal.GameProfile {
            return getInstance(gameProfileCreate())!!
        }

        fun copyInstance(profile: ch.dragondreams.delauncher.launcher.internal.GameProfile):
                ch.dragondreams.delauncher.launcher.internal.GameProfile {
            return getInstance(gameProfileCopy(profile.nativeProfile))!!
        }

        private fun dropInstance(nativeProfile: Long){
            mapProfiles.remove(nativeProfile)
        }
    }
}
