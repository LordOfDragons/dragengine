package ch.dragondreams.delauncher

import android.R
import android.os.Bundle
import android.util.Log
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule
import ch.dragondreams.delauncher.launcher.EngineModuleParameter
import ch.dragondreams.delauncher.launcher.Game
import ch.dragondreams.delauncher.launcher.GameProfile
import ch.dragondreams.delauncher.launcher.GameRunParams
import ch.dragondreams.delauncher.launcher.internal.GameActivityAdapter
import ch.dragondreams.delauncher.launcher.internal.RunGameHandler
import ch.dragondreams.delauncher.ui.main.FragmentInitEngine
import com.google.androidgamesdk.GameActivity
import java.lang.System.loadLibrary


class RunDelgaActivity : GameActivity(),
    FragmentInitEngine.Interface {

    class RunLauncherListener(
        private val activity: RunDelgaActivity
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
        RunGame
    }

    private var launcher: DragengineLauncher? = null
    private var state = State.InitEngine
    private var delgaGames: MutableList<Game> = mutableListOf()
    private var delgaGame: Game? = null
    private var delgaPath: String? = null
    private var hasPatchIdentifier = false
    private var patchIdentifier: String? = null
    private val moduleParameters: MutableList<EngineModuleParameter> = mutableListOf()
    private var runParams: GameRunParams = GameRunParams()

    override fun getLauncher(): DragengineLauncher {
        if (launcher == null) {
            launcher = DragengineLauncher(this)
            launcher!!.addListener(RunLauncherListener(this))
        }
        return launcher!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        loadLibrary("game_activity_adapter")
        super.onCreate(savedInstanceState)
        /*
        enableEdgeToEdge()
        setContentView(R.layout.activity_run_delga)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
        */

        logInfo("RunDelgaActivity", listOf(
            "action='${intent.action}'",
            "type='${intent.type}'",
            "scheme='${intent.scheme}'",
            "data='${intent.data}'").joinToString(" "))

        if (intent.action == "ch.dragondreams.delauncher.LAUNCH_DELGA"
            || intent.action == "android.intent.action.VIEW") {
            getLauncher() // force create launcher if not created already

            supportFragmentManager.beginTransaction()
                .add(R.id.content, FragmentInitEngine()).commit()

        } else {
            finish()
        }
    }

    override fun onDestroy() {
        GameActivityAdapter().setHandler(0L)

        delgaGame?.dispose()
        delgaGame = null

        delgaGames.forEach { g -> g.dispose() }
        delgaGames.clear()

        launcher?.dispose()
        launcher = null

        super.onDestroy()
    }

    private fun onEngineReady() {
        if (state != State.InitEngine) {
            return
        }

        state = State.RunGame
        processIntentLaunchDelga()
    }

    private fun processIntentLaunchDelga() {
        // we can not use this check since google file app sends ACTION_VIEW intents
        // with type "application/octet-stream" and the URI path obfuscated to stuff like
        // "com.google.android.apps.nbu.files.provider/2/1000000579". this hides the file
        // pattern and also the filename. lucky us though we do not need the correct
        // filename for launching to work. so much for overzealous security ideas
        /*
        if (intent.dataString?.endsWith(".delga") != true) {
            finish()
            return
        }
        */

        if(!locateGame()){
            finish()
            return
        }

        if(delgaGame?.canRun != true){
            logProblems()
            return
        }

        if(!locateProfile()){
            return
        }

        startGame()
    }

    private fun locateGame(): Boolean {
        when (intent.data?.scheme) {
            "content" -> {
                contentResolver.openAssetFileDescriptor(intent.data!!, "r").use { afd ->
                    if (afd == null) {
                        logError("processIntentLaunchDelga",
                            "AssetFileDescriptor is null")
                        return false
                    }

                    val startOffset = afd.startOffset
                    val length = afd.length
                    logInfo("processIntentLaunchDelga",
                        "startOffset=${afd.startOffset} length=${afd.length}"
                    )

                    val fd = afd.parcelFileDescriptor?.detachFd()
                    if (fd == null) {
                        logError("processIntentLaunchDelga",
                            "Can not detach file descriptor")
                        return false
                    }

                    try {
                        launcher!!.vfsContainerAddFd(
                            "/$VFS_FDS_DELGA_FILENAME",
                            fd,
                            startOffset.toInt(),
                            length.toInt()
                        )
                    } catch (e: Exception) {
                        logError("processIntentLaunchDelga",
                            "Failed adding VFS container", e)
                        return false
                    }

                    logInfo("processIntentLaunchDelga",
                        "VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            "file" -> {
                contentResolver.openFileDescriptor(intent.data!!, "r").use { pfd ->
                    if (pfd == null) {
                        logError("processIntentLaunchDelga",
                            "ParcelFileDescriptor is null")
                        return false
                    }

                    val length = pfd.statSize
                    logError("processIntentLaunchDelga", "length=${length}")

                    val fd = pfd.detachFd()

                    try {
                        launcher!!.vfsContainerAddFd(
                            "/$VFS_FDS_DELGA_FILENAME",
                            fd,
                            0,
                            length.toInt()
                        )
                    } catch (e: Exception) {
                        logError("processIntentLaunchDelga",
                            "Failed adding VFS container", e)
                        return false
                    }

                    logInfo("processIntentLaunchDelga", "VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            else -> {
                logError("processIntentLaunchDelga", "Unsupported scheme")
                return false
            }
        }

        if (delgaPath == null) {
            logError("processIntentLaunchDelga", "DELGA path is null")
            return false
        }

        delgaGames.forEach { g -> g.dispose() }
        delgaGames.clear()
        delgaGames.addAll(launcher!!.readDelgaGames(delgaPath!!))
        logInfo("processIntentLaunchDelga", "DELGA loaded")

        if (delgaGames.isEmpty()) {
            logError("processIntentLaunchDelga",
                "No game definition found in DELGA file")
            return false
        }

        delgaGames.subList(1, delgaGames.size).forEach { g -> g.dispose() }

        delgaGame?.dispose()
        delgaGame = delgaGames[0].reference()

        // load configuration if the game is not installed. this allows to keep the parameter
        // changes alive done by the player inside the game
        val identifier = delgaGame!!.identifier
        if (launcher!!.games.find { g -> g.identifier == identifier } == null) {
            delgaGame?.loadConfig()
        }

        delgaGame?.updateConfig()
        delgaGame?.verifyRequirements()
        delgaGame?.updateStatus()
        logInfo("processIntentLaunchDelga",
            "DELGA status ${delgaGame?.canRun}")
        return true
    }

    private fun logProblems() {
        val game = delgaGame!!
        val launcher = launcher!!

        logInfo("logProblems",
            "Game '${game.aliasIdentifier}'(${game.identifier}) has the following problems:")
        game.fileFormats.forEach { f ->
            if(!f.supported){
                if(launcher.isModuleSingleType(f.type)){
                    logInfo("logProblems",
                        "- File Format '${f.pattern}' defines single type ${EngineModule.mapTypeName[f.type]}")
                }else{
                    logInfo("logProblems",
                        "- File Format '${f.pattern}' is not supported by any loaded modules")
                }
            }
        }
        if(!game.scriptModuleFound){
            printModuleProblem(game.scriptModule, EngineModule.Type.Script);
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
        val game = delgaGame!!

        // locate the profile to run
        var profile: GameProfile?

        profile = game.getProfileToUse(launcher!!)
        if(profile == null){
            logError("locateProfile", "No game profile found")
            return false
        }

        if(!profile.valid){
            printProfileProblems(profile)
            return false
        }

        // determine patch to use
        if(delgaGame!!.useCustomPatch.isNotEmpty()){
            hasPatchIdentifier = true
            patchIdentifier = game.useCustomPatch

        }else if(!delgaGame!!.useLatestPatch){
            hasPatchIdentifier = true
            patchIdentifier = null
        }

        // udpate the run parameters
        runParams.gameProfile = profile

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
            arguments = delgaGame!!.runArguments
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
        val game = delgaGame!!
        var profile: GameProfile? = game.customProfile

        if(profile == null){
            profile = GameProfile.copyInstance(runParams.gameProfile!!)
            game.customProfile = profile
            game.storeConfig()
        }

        // update custom profile
        //TODO: profile->GetModules().Update( *pModuleParameters );

        // use custom profile
        runParams.gameProfile = profile
    }

    private fun startGame() {
        val game = delgaGame!!

        // start the game
        logInfo("startGame",
            "Cache application ID = '${game.identifier}'")
        logInfo("startGame",
            "Starting game '${game.title}' using profile '${runParams.gameProfile?.name}'");

        loadLibrary("run_game_handler")
        GameActivityAdapter().setHandler(RunGameHandler().createHandler(
            game.nativeGame, runParams.toNative()))

        /*
        const delEngineInstanceDirect::Factory::Ref factory(
                delEngineInstanceDirect::Factory::Ref::New(
                    new delEngineInstanceDirect::Factory( engineLogger ) ) );
         */
    }

    private fun logError(function: String, message: String){
        val m = "$function: $message"
        Log.e(TAG, m)
        launcher?.logError(m)
    }

    private fun logError(function: String, message: String, exception: Exception){
        val m = "$function: $message"
        Log.e(TAG, m, exception)
        launcher?.logError(listOf(m, exception.toString(),
            exception.stackTraceToString()).joinToString("\n"))
    }

    private fun logInfo(function: String, message: String){
        val m = "$function: $message"
        Log.i(TAG, m)
        launcher?.logInfo(m)
    }

    companion object {
        const val TAG = "RunDelgaActivity"
        private const val VFS_FDS_PATH = "/fds"
        private const val VFS_FDS_DELGA_FILENAME = "game.delga"
        private const val VFS_FDS_DELGA_PATH = "$VFS_FDS_PATH/$VFS_FDS_DELGA_FILENAME"
    }
}
