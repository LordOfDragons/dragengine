package ch.dragondreams.delauncher

import android.util.Log
import android.view.SurfaceView
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule
import ch.dragondreams.delauncher.launcher.EngineModuleParameter
import ch.dragondreams.delauncher.launcher.Game
import ch.dragondreams.delauncher.launcher.GameProfile
import ch.dragondreams.delauncher.launcher.GameRunParams

class RunGameShared(
    val loggerTag: String
) {
    var launcher: DragengineLauncher? = null
    var surfaceView: SurfaceView? = null

    /**
     * Game instance. Automatically handles release/retain calls.
     */
    var game: Game? = null
        set(value) {
            if (field == value){
               return
            }

            field?.release()
            field = value?.retain()
        }

    var hasPatchIdentifier = false
    var patchIdentifier: String? = null
    val moduleParameters: MutableList<EngineModuleParameter> = mutableListOf()
    var runParams: GameRunParams = GameRunParams()



    fun dispose() {
        game = null

        launcher?.dispose()
        launcher = null
    }



    /**
     * Load configuration if the game is not installed. this allows to keep the parameter
     * changes alive done by the player inside the game.
     */
    fun loadGameConfig(){
        val l = launcher!!
        val g = game!!

        val identifier = g.identifier
        if (l.games.find { g2 -> g2.identifier == identifier } == null) {
            g.loadConfig()
        }

        g.updateConfig()
        g.verifyRequirements()
        g.updateStatus()
        logInfo("loadGameConfig", "Game status: canRun=${g.canRun}")
    }

    /**
     * Locate game profile to use.
     */
    fun locateProfile(profile: GameProfile?): Boolean{
        val sv = surfaceView!!
        val l = launcher!!
        val g = game!!

        // locate the profile to run
        val useProfile = profile ?: g.getProfileToUse(l)
        if(useProfile == null){
            logError("locateProfile", "No game profile found")
            return false
        }

        if(!useProfile.valid){
            logProfileProblems(useProfile)
            return false
        }

        // determine patch to use
        if(g.useCustomPatch.isNotEmpty()){
            hasPatchIdentifier = true
            patchIdentifier = g.useCustomPatch

        }else if(!g.useLatestPatch){
            hasPatchIdentifier = true
            patchIdentifier = null
        }

        // update the run parameters
        runParams.gameProfile = useProfile
        runParams.width = sv.width
        runParams.height = sv.height

        /*
        var error: String
        if(!runParams.findPatches(g, g.useLatestPatch, pPatchIdentifier, error)){
            logError("locateProfile", error.toString())
            return false
        }
        */

        updateRunArguments()
        applyCustomModuleParameters() // potentially changes game profile set in run parameters
        return true
    }

    /**
     * Update run arguments.
     */
    fun updateRunArguments(){
        val g = game!!

        val profile = runParams.gameProfile!!
        var arguments: String

        if(profile.replaceRunArguments){
            arguments = profile.runArguments
        }else{
            arguments = g.runArguments
            if(arguments.isNotEmpty()){
                arguments += " "
            }
            arguments += profile.runArguments
        }

        runParams.runArguments = arguments
    }

    /**
     * Apply custom module parameters.
     */
    fun applyCustomModuleParameters(){
        if(moduleParameters.isEmpty()){
            return
        }

        // ensure custom profile exists and is initialized with profile used to run the game
        val g = game!!
        var profile: GameProfile? = g.customProfile

        if(profile == null){
            profile = GameProfile.copyInstance(runParams.gameProfile!!)
            g.customProfile = profile
            g.storeConfig()
        }

        // update custom profile
        //TODO: profile->GetModules().Update( *pModuleParameters );

        // use custom profile
        runParams.gameProfile = profile
    }


    /**
     * Log game problems.
     */
    fun logGameProblems() {
        val g = game!!
        val l = launcher!!

        logInfo("logProblems",
            "Game '${g.aliasIdentifier}'(${g.identifier}) has the following problems:")
        g.fileFormats.forEach { f ->
            if(!f.supported){
                if(l.isModuleSingleType(f.type)){
                    logInfo("logProblems",
                        "- File Format '${f.pattern}' defines single type ${EngineModule.mapTypeName[f.type]}")
                }else{
                    logInfo("logProblems",
                        "- File Format '${f.pattern}' is not supported by any loaded modules")
                }
            }
        }
        if(!g.scriptModuleFound){
            logModuleProblems(g.scriptModule, EngineModule.Type.Script)
        }
    }

    /**
     * Log engine module problems.
     */
    fun logModuleProblems(name: String, type: EngineModule.Type) {
        val l = launcher!!

        val module = l.engineModules.find { m -> m.name == name }
        val typeName = EngineModule.mapTypeName[type]

        if(module == null){
            logInfo("logModuleProblems",
                "$typeName module '$name' does not exist")

        }else if(module.type != type){
            logInfo("logModuleProblems",
                "- Module '$name' is not a $typeName module")

        }else if(module.status != EngineModule.Status.Ready){
            val errorCode = EngineModule.mapErrorCode[module.errorCode]
            val reason = EngineModule.mapErrorText[errorCode] ?: "Unknown problem"
            logInfo("logModuleProblems",
                "- $typeName module '${name}' is not working ($reason)")
        }
    }

    /**
     * Log profile problems.
     */
    fun logProfileProblems(profile: GameProfile){
        logInfo("logProfileProblems",
            "Profile '${profile.name}' has the following problems:")
        logModuleProblems(profile.moduleGraphic, EngineModule.Type.Graphic)
        logModuleProblems(profile.moduleInput, EngineModule.Type.Input)
        logModuleProblems(profile.modulePhysics, EngineModule.Type.Physics)
        logModuleProblems(profile.moduleAnimator, EngineModule.Type.Animator)
        logModuleProblems(profile.moduleAI, EngineModule.Type.AI)
        logModuleProblems(profile.moduleCrashRecovery, EngineModule.Type.CrashRecovery)
        logModuleProblems(profile.moduleAudio, EngineModule.Type.Audio)
        logModuleProblems(profile.moduleSynthesizer, EngineModule.Type.Synthesizer)
        logModuleProblems(profile.moduleNetwork, EngineModule.Type.Network)
        logModuleProblems(profile.moduleVR, EngineModule.Type.VR)
    }

    fun logError(function: String, message: String){
        val m = "$function: $message"
        launcher?.logError(m) ?: Log.e(loggerTag, m)
    }

    fun logError(function: String, message: String, exception: Exception){
        val m = "$function: $message"
        launcher?.logError(listOf(m, exception.toString(),
            exception.stackTraceToString()).joinToString("\n"))
                ?: Log.e(loggerTag, m, exception)
    }

    fun logInfo(function: String, message: String){
        val m = "$function: $message"
        launcher?.logInfo(m) ?: Log.i(loggerTag, m)
    }
}
