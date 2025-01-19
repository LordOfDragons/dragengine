package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.GameProfile

data class GameProfileConfig(
    var name: String = "",

    var moduleGraphic: String = "",
    var moduleInput: String = "",
    var modulePhysics: String = "",
    var moduleAnimator: String = "",
    var moduleAI: String = "",
    var moduleCrashRecovery: String = "",
    var moduleAudio: String = "",
    var moduleSynthesizer: String = "",
    var moduleNetwork: String = "",
    var moduleVR: String = "",

    var moduleGraphicVersion: String = "",
    var moduleInputVersion: String = "",
    var modulePhysicsVersion: String = "",
    var moduleAnimatorVersion: String = "",
    var moduleAIVersion: String = "",
    var moduleCrashRecoveryVersion: String = "",
    var moduleAudioVersion: String = "",
    var moduleSynthesizerVersion: String = "",
    var moduleNetworkVersion: String = "",
    var moduleVRVersion: String = "",

    var disableModuleVersions: Array<GameProfileModuleVersion> = emptyArray(),

    var modules: Array<GameProfileModule> = emptyArray(),

    var runArgs: String = "",
    var replaceRunArgs: Boolean = false
){
    fun update(game: GameProfile) {
        game.name = name

        game.moduleGraphic = moduleGraphic
        game.moduleInput = moduleInput
        game.modulePhysics = modulePhysics
        game.moduleAnimator = moduleAnimator
        game.moduleAI = moduleAI
        game.moduleCrashRecovery = moduleCrashRecovery
        game.moduleAudio = moduleAudio
        game.moduleSynthesizer = moduleSynthesizer
        game.moduleNetwork = moduleNetwork
        game.moduleVR = moduleVR

        game.moduleGraphicVersion = moduleGraphicVersion
        game.moduleInputVersion = moduleInputVersion
        game.modulePhysicsVersion = modulePhysicsVersion
        game.moduleAnimatorVersion = moduleAnimatorVersion
        game.moduleAIVersion = moduleAIVersion
        game.moduleCrashRecoveryVersion = moduleCrashRecoveryVersion
        game.moduleAudioVersion = moduleAudioVersion
        game.moduleSynthesizerVersion = moduleSynthesizerVersion
        game.moduleNetworkVersion = moduleNetworkVersion
        game.moduleVRVersion = moduleVRVersion

        game.disableModuleVersions.clear()
        disableModuleVersions.forEach { e -> game.disableModuleVersions[e.name] = e.version }

        game.modules.clear()
        modules.forEach { m ->
            val module = GameProfile.Module()
            module.name = m.name
            m.parameters.forEach { p -> module.parameters[p.name] = p.value }
            game.modules.add(module)
        }

        game.runArguments = runArgs
        game.replaceRunArguments = replaceRunArgs
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as GameProfileConfig

        if (name != other.name) return false
        if (moduleGraphic != other.moduleGraphic) return false
        if (moduleInput != other.moduleInput) return false
        if (modulePhysics != other.modulePhysics) return false
        if (moduleAnimator != other.moduleAnimator) return false
        if (moduleAI != other.moduleAI) return false
        if (moduleCrashRecovery != other.moduleCrashRecovery) return false
        if (moduleAudio != other.moduleAudio) return false
        if (moduleSynthesizer != other.moduleSynthesizer) return false
        if (moduleNetwork != other.moduleNetwork) return false
        if (moduleVR != other.moduleVR) return false
        if (moduleGraphicVersion != other.moduleGraphicVersion) return false
        if (moduleInputVersion != other.moduleInputVersion) return false
        if (modulePhysicsVersion != other.modulePhysicsVersion) return false
        if (moduleAnimatorVersion != other.moduleAnimatorVersion) return false
        if (moduleAIVersion != other.moduleAIVersion) return false
        if (moduleCrashRecoveryVersion != other.moduleCrashRecoveryVersion) return false
        if (moduleAudioVersion != other.moduleAudioVersion) return false
        if (moduleSynthesizerVersion != other.moduleSynthesizerVersion) return false
        if (moduleNetworkVersion != other.moduleNetworkVersion) return false
        if (moduleVRVersion != other.moduleVRVersion) return false
        if (!disableModuleVersions.contentEquals(other.disableModuleVersions)) return false
        if (!modules.contentEquals(other.modules)) return false
        if (runArgs != other.runArgs) return false
        if (replaceRunArgs != other.replaceRunArgs) return false

        return true
    }

    override fun hashCode(): Int {
        var result = name.hashCode()
        result = 31 * result + moduleGraphic.hashCode()
        result = 31 * result + moduleInput.hashCode()
        result = 31 * result + modulePhysics.hashCode()
        result = 31 * result + moduleAnimator.hashCode()
        result = 31 * result + moduleAI.hashCode()
        result = 31 * result + moduleCrashRecovery.hashCode()
        result = 31 * result + moduleAudio.hashCode()
        result = 31 * result + moduleSynthesizer.hashCode()
        result = 31 * result + moduleNetwork.hashCode()
        result = 31 * result + moduleVR.hashCode()
        result = 31 * result + moduleGraphicVersion.hashCode()
        result = 31 * result + moduleInputVersion.hashCode()
        result = 31 * result + modulePhysicsVersion.hashCode()
        result = 31 * result + moduleAnimatorVersion.hashCode()
        result = 31 * result + moduleAIVersion.hashCode()
        result = 31 * result + moduleCrashRecoveryVersion.hashCode()
        result = 31 * result + moduleAudioVersion.hashCode()
        result = 31 * result + moduleSynthesizerVersion.hashCode()
        result = 31 * result + moduleNetworkVersion.hashCode()
        result = 31 * result + moduleVRVersion.hashCode()
        result = 31 * result + disableModuleVersions.contentHashCode()
        result = 31 * result + modules.contentHashCode()
        result = 31 * result + runArgs.hashCode()
        result = 31 * result + replaceRunArgs.hashCode()
        return result
    }
}
