package ch.dragondreams.delauncher

import android.content.ContentResolver
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.SurfaceHolder
import android.view.View
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.Game
import ch.dragondreams.delauncher.launcher.internal.GameActivityAdapter
import ch.dragondreams.delauncher.launcher.internal.Launcher
import ch.dragondreams.delauncher.launcher.internal.RunGameHandler
import ch.dragondreams.delauncher.ui.FragmentInitEngine
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

    class HandlerListener(
        private val owner: RunDelgaActivity
    ) : RunGameHandler.Listener{
        private val handler = Handler(Looper.getMainLooper())

        override fun stateChanged(state: Int) {
            if(!owner.isDestroyingView) {
                handler.post { owner.handlerStateChanged(RunGameHandler.mapState[state]!!) }
            }
        }
    }

    class AssetFileDescriptorProducer(
        private val runGameShared: RunGameShared,
        private val contentResolver: ContentResolver,
        private val intent: Intent
    ) : Launcher.FileDescriptorProducer{
        override fun produceFileDescriptor(): Int {
            contentResolver.openAssetFileDescriptor(intent.data!!, "r").use { afd ->
                if (afd == null) {
                    runGameShared.logError("produceFileDescriptor",
                        "AssetFileDescriptor is null")
                    return 0
                }

                val fd = afd.parcelFileDescriptor?.detachFd()
                if (fd == null) {
                    runGameShared.logError("produceFileDescriptor",
                        "Can not detach file descriptor")
                    return 0
                }

                runGameShared.logInfo("produceFileDescriptor",
                    "File descriptor created")
                return fd
            }
        }
    }

    class FileFileDescriptorProducer(
        private val runGameShared: RunGameShared,
        private val contentResolver: ContentResolver,
        private val intent: Intent
    ) : Launcher.FileDescriptorProducer{
        override fun produceFileDescriptor(): Int {
            contentResolver.openFileDescriptor(intent.data!!, "r").use { pfd ->
                if (pfd == null) {
                    runGameShared.logError("produceFileDescriptor",
                        "ParcelFileDescriptor is null")
                    return 0
                }

                runGameShared.logInfo("produceFileDescriptor",
                    "File descriptor created")
                return pfd.detachFd()
            }
        }
    }

    enum class State {
        InitEngine,
        RunGame
    }

    private val shared = RunGameShared(TAG)
    private var state = State.InitEngine
    private var delgaGames: MutableList<Game> = mutableListOf()
    private var delgaPath: String? = null
    private var runGameHandler: RunGameHandler? = null
    @Volatile
    private var isDestroyingView = false

    override fun getLauncher(): DragengineLauncher {
        if (shared.launcher == null) {
            shared.launcher = DragengineLauncher(this, mSurfaceView)
            shared.launcher?.logFilename = "delauncher"
            shared.launcher?.addListener(RunLauncherListener(this))
            shared.launcher?.initLauncher()
        }
        return shared.launcher!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        loadLibrary("game_activity_adapter")
        super.onCreate(savedInstanceState)

        shared.logInfo("RunDelgaActivity", listOf(
            "action='${intent.action}'",
            "type='${intent.type}'",
            "scheme='${intent.scheme}'",
            "data='${intent.data}'").joinToString(" "))

        if (intent.action == ACTION_LAUNCH_DELGA
            || intent.action == "android.intent.action.VIEW") {
            mSurfaceView.holder.addCallback(object : SurfaceHolder.Callback {
                override fun surfaceCreated(holder: SurfaceHolder){
                    shared.surfaceView = mSurfaceView
                    getLauncher() // force create launcher if not created already
                    supportFragmentManager.beginTransaction()
                        .add(android.R.id.content, FragmentInitEngine()).commit()
                }

                override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int){
                }

                override fun surfaceDestroyed(holder: SurfaceHolder){
                    stopApplicationWait()
                }
            })

        } else {
            finish()
        }
    }

    override fun onDestroy() {
        isDestroyingView = true
        stopApplicationWait()

        GameActivityAdapter().setHandler(0L)
        runGameHandler?.dispose()
        runGameHandler = null

        delgaGames.forEach { g -> g.release() }
        delgaGames.clear()

        shared.dispose()
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

        if(shared.game?.canRun != true){
            shared.logGameProblems()
            return
        }

        if(!shared.locateProfile(null)){
            return
        }

        startGame()
    }

    private fun locateGame(): Boolean {
        when (intent.data?.scheme) {
            "content" -> {
                contentResolver.openAssetFileDescriptor(intent.data!!, "r").use { afd ->
                    if (afd == null) {
                        shared.logError("locateGame",
                            "AssetFileDescriptor is null")
                        return false
                    }

                    val startOffset = afd.startOffset
                    val length = afd.length
                    shared.logInfo("locateGame",
                        "startOffset=${afd.startOffset} length=${afd.length}"
                    )

                    try {
                        shared.launcher?.vfsContainerAddFd(
                            "/$VFS_FDS_DELGA_FILENAME",
                            AssetFileDescriptorProducer(shared, contentResolver, intent),
                            startOffset.toInt(),
                            length.toInt()
                        )
                    } catch (e: Exception) {
                        shared.logError("locateGame",
                            "Failed adding VFS container", e)
                        return false
                    }

                    shared.logInfo("locateGame",
                        "VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            "file" -> {
                contentResolver.openFileDescriptor(intent.data!!, "r").use { pfd ->
                    if (pfd == null) {
                        shared.logError("locateGame",
                            "ParcelFileDescriptor is null")
                        return false
                    }

                    val length = pfd.statSize
                    shared.logError("locateGame", "length=${length}")

                    val fd = pfd.detachFd()

                    try {
                        shared.launcher?.vfsContainerAddFd(
                            "/$VFS_FDS_DELGA_FILENAME",
                            FileFileDescriptorProducer(shared, contentResolver, intent),
                            0,
                            length.toInt()
                        )
                    } catch (e: Exception) {
                        shared.logError("locateGame",
                            "Failed adding VFS container", e)
                        return false
                    }

                    shared.logInfo("locateGame", "VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            else -> {
                shared.logError("locateGame", "Unsupported scheme")
                return false
            }
        }

        if (delgaPath == null) {
            shared.logError("locateGame", "DELGA path is null")
            return false
        }

        delgaGames.forEach { g -> g.release() }
        delgaGames.clear()
        delgaGames.addAll(shared.launcher!!.readDelgaGames(delgaPath!!))
        shared.logInfo("locateGame", "DELGA loaded")

        if (delgaGames.isEmpty()) {
            shared.logError("locateGame",
                "No game definition found in DELGA file")
            return false
        }

        shared.game = delgaGames[0]

        delgaGames.forEach { g -> g.release() }
        delgaGames.clear()

        shared.loadGameConfig()

        val ownerPackage = intent.getStringExtra(EXTRA_OWNER_PACKAGE)
        if (ownerPackage != null) {
            shared.game?.customProperties?.put(Game.PROPERTY_OWNER_PACKAGE, ownerPackage)
            shared.game?.storeConfig()
        }

        return true
    }

    private fun startGame() {
        val game = shared.game!!

        // start the game
        shared.logInfo("startGame",
            "Cache application ID = '${game.identifier}'")
        shared.logInfo("startGame",
            "Starting game '${game.title}' using profile '${shared.runParams.gameProfile?.name}'");

        loadLibrary("run_game_handler")
        runGameHandler = RunGameHandler(shared.launcher!!, game,
            shared.runParams, HandlerListener(this))
        GameActivityAdapter().setHandler(runGameHandler!!.nativeHandler)

        runOnUiThread {
            //window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN)
            window.decorView.systemUiVisibility += (
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_FULLSCREEN)
        }
    }

    fun stopApplicationWait() {
        shared.logInfo(TAG, "stopApplication")
        runGameHandler?.stopGame()
        runGameHandler?.waitForState(RunGameHandler.State.GameStopped)
    }

    private fun handlerStateChanged(state: RunGameHandler.State) {
        if(state == RunGameHandler.State.GameStopped){
            GameActivityAdapter().setHandler(0L)
            runGameHandler?.dispose()
            runGameHandler = null
        }
    }

    companion object {
        const val TAG = "RunDelgaActivity"
        private const val VFS_FDS_PATH = "/fds"
        private const val VFS_FDS_DELGA_FILENAME = "game.delga"
        private const val VFS_FDS_DELGA_PATH = "$VFS_FDS_PATH/$VFS_FDS_DELGA_FILENAME"

        const val ACTION_LAUNCH_DELGA = "ch.dragondreams.delauncher.LAUNCH_DELGA"

        const val EXTRA_OWNER_PACKAGE = "ownerPackage"
        const val EXTRA_GAME_ID = "launchDelga.gameId"

        const val MIME_TYPE_DELGA = "application/dragengine-delga"
    }
}
