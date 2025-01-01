package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.GameProfile

data class GameProfileInfo(
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
}
