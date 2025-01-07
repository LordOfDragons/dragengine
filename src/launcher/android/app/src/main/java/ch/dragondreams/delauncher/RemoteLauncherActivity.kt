package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import ch.dragondreams.delauncher.RunDelgaActivity.Companion
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule
import ch.dragondreams.delauncher.launcher.EngineModuleParameter
import ch.dragondreams.delauncher.launcher.Game
import ch.dragondreams.delauncher.launcher.GameProfile
import ch.dragondreams.delauncher.launcher.GameRunParams
import ch.dragondreams.delauncher.launcher.internal.GameActivityAdapter
import ch.dragondreams.delauncher.launcher.internal.RemoteLauncherHandler
import ch.dragondreams.delauncher.launcher.internal.RunGameHandler
import ch.dragondreams.delauncher.ui.main.FragmentInitEngine
import com.google.androidgamesdk.GameActivity
import java.lang.System.loadLibrary

class RemoteLauncherActivity : GameActivity(),
    FragmentInitEngine.Interface
{
    class RemoteLauncherListener(
        private val activity: RemoteLauncherActivity
    ) : DragengineLauncher.DefaultListener() {
        override fun stateChanged(launcher: DragengineLauncher) {
            Log.i(TAG, "stateChanged: " + launcher.state)
            if (launcher.state == DragengineLauncher.State.EngineReady) {
                activity.onEngineReady()
            }
        }
    }

    enum class State {
        InitEngine,
        Idle,
        RunGame
    }

    private var launcher: DragengineLauncher? = null
    private var state = State.InitEngine
    private var handler: RemoteLauncherHandler? = null
    private var runParams: GameRunParams = GameRunParams()
    private var game: Game? = null
    private var hasPatchIdentifier = false
    private var patchIdentifier: String? = null
    private val moduleParameters: MutableList<EngineModuleParameter> = mutableListOf()

    override fun getLauncher(): DragengineLauncher {
        if (launcher == null) {
            launcher = DragengineLauncher(this, mSurfaceView)
            launcher!!.addListener(RemoteLauncherListener(this))
        }
        return launcher!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        loadLibrary("game_activity_adapter")
        super.onCreate(savedInstanceState)

        mSurfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(holder: SurfaceHolder){
                getLauncher() // force create launcher if not created already
                supportFragmentManager.beginTransaction()
                    .add(android.R.id.content, FragmentInitEngine()).commit()
            }

            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int){
            }

            override fun surfaceDestroyed(holder: SurfaceHolder){
            }
        })
    }

    override fun onDestroy() {
        GameActivityAdapter().setHandler(0L)
        handler?.dispose()
        handler = null

        launcher?.dispose()
        launcher = null

        super.onDestroy()
    }

    private fun onEngineReady() {
        if (state != State.InitEngine) {
            return
        }
        state = State.Idle
    }

    private fun runGame() {

    }

    private fun prepareGame(): Boolean{
        if(game == null){
            return false
        }

        // load configuration if the game is not installed. this allows to keep the parameter
        // changes alive done by the player inside the game
        val identifier = game?.identifier
        if (launcher!!.games.find { g -> g.identifier == identifier } == null) {
            game?.loadConfig()
        }

        game?.updateConfig()
        game?.verifyRequirements()
        game?.updateStatus()
        logInfo("processIntentLaunchDelga", "DELGA status ${game?.canRun}")
        return true
    }

    private fun logProblems() {
        logInfo("logProblems",
            "Game '${game?.aliasIdentifier}'(${game?.identifier}) has the following problems:")
        game!!.fileFormats.forEach { f ->
            if(!f.supported){
                if(launcher!!.isModuleSingleType(f.type)){
                    logInfo("logProblems",
                        "- File Format '${f.pattern}' defines single type ${EngineModule.mapTypeName[f.type]}")
                }else{
                    logInfo("logProblems",
                        "- File Format '${f.pattern}' is not supported by any loaded modules")
                }
            }
        }
        if(!game!!.scriptModuleFound){
            printModuleProblem(game!!.scriptModule, EngineModule.Type.Script);
        }
    }

    private fun printModuleProblem(name: String, type: EngineModule.Type) {
        val module = launcher?.engineModules?.find { m -> m.name == name }
        val typeName = EngineModule.mapTypeName[type]

        if(module == null){
            logInfo("printModuleProblem",
                "$typeName module '$name' does not exist")

        }else if(module.type != type){
            logInfo("printModuleProblem",
                "- Module '$name' is not a $typeName module")

        }else if(module.status != EngineModule.Status.Ready){
            val errorCode = EngineModule.mapErrorCode[module.errorCode]
            val reason = EngineModule.mapErrorText[errorCode] ?: "Unknown problem"
            logInfo("printModuleProblem",
                "- $typeName module '${name}' is not working ($reason)")
        }
    }

    private fun locateProfile(): Boolean{
        // locate the profile to run
        var profile: GameProfile?

        profile = game?.getProfileToUse(launcher!!)
        if(profile == null){
            logError("locateProfile", "No game profile found")
            return false
        }

        if(!profile.valid){
            printProfileProblems(profile)
            return false
        }

        // determine patch to use
        if(game!!.useCustomPatch.isNotEmpty()){
            hasPatchIdentifier = true
            patchIdentifier = game?.useCustomPatch

        }else if(!game!!.useLatestPatch){
            hasPatchIdentifier = true
            patchIdentifier = null
        }

        // udpate the run parameters
        runParams.gameProfile = profile
        runParams.width = mSurfaceView.width
        runParams.height = mSurfaceView.height

        /*
        var error: String
        if(!runParams.findPatches(delgaGame, delgaGame.useLatestPatch, pPatchIdentifier, error)){
            logger.LogError( LOGSOURCE, error.GetString() );
            return false;
        }
        */

        updateRunArguments()
        applyCustomModuleParameters(); // potentially changes game profile set in run parameters
        return true;
    }

    private fun printProfileProblems(profile: GameProfile){
        logInfo("printProfileProblems",
            "Profile '${profile.name}' has the following problems:")
        printModuleProblem(profile.moduleGraphic, EngineModule.Type.Graphic)
        printModuleProblem(profile.moduleInput, EngineModule.Type.Input)
        printModuleProblem(profile.modulePhysics, EngineModule.Type.Physics)
        printModuleProblem(profile.moduleAnimator, EngineModule.Type.Animator)
        printModuleProblem(profile.moduleAI, EngineModule.Type.AI)
        printModuleProblem(profile.moduleCrashRecovery, EngineModule.Type.CrashRecovery)
        printModuleProblem(profile.moduleAudio, EngineModule.Type.Audio)
        printModuleProblem(profile.moduleSynthesizer, EngineModule.Type.Synthesizer)
        printModuleProblem(profile.moduleNetwork, EngineModule.Type.Network)
        printModuleProblem(profile.moduleVR, EngineModule.Type.VR)
    }

    private fun updateRunArguments(){
        val profile = runParams.gameProfile!!
        var arguments: String

        if(profile.replaceRunArguments){
            arguments = profile.runArguments
        }else{
            arguments = game!!.runArguments
            if(arguments.isNotEmpty()){
                arguments += " "
            }
            arguments += profile.runArguments
        }

        runParams.runArguments = arguments
    }

    private fun applyCustomModuleParameters(){
        if(moduleParameters.isEmpty()){
            return
        }

        // ensure custom profile exists and is initialized with profile used to run the game
        var profile: GameProfile? = game?.customProfile

        if(profile == null){
            profile = GameProfile.copyInstance(runParams.gameProfile!!)
            game?.customProfile = profile
            game?.storeConfig()
        }

        // update custom profile
        //TODO: profile->GetModules().Update( *pModuleParameters );

        // use custom profile
        runParams.gameProfile = profile
    }

    private fun startGame() {
        // start the game
        logInfo("startGame",
            "Cache application ID = '${game?.identifier}'")
        logInfo("startGame",
            "Starting game '${game?.title}' using profile '${runParams.gameProfile?.name}'");

        loadLibrary("run_game_handler")
        handler = RemoteLauncherHandler(launcher!!)
        GameActivityAdapter().setHandler(handler!!.nativeHandler)
    }

    private fun logError(function: String, message: String){
        val m = "$function: $message"
        Log.e(RunDelgaActivity.TAG, m)
        launcher?.logError(m)
    }

    private fun logError(function: String, message: String, exception: Exception){
        val m = "$function: $message"
        Log.e(RunDelgaActivity.TAG, m, exception)
        launcher?.logError(listOf(m, exception.toString(),
            exception.stackTraceToString()).joinToString("\n"))
    }

    private fun logInfo(function: String, message: String){
        val m = "$function: $message"
        Log.i(RunDelgaActivity.TAG, m)
        launcher?.logInfo(m)
    }

    companion object {
        const val TAG = "RemoteLauncherActivity"
    }
}
