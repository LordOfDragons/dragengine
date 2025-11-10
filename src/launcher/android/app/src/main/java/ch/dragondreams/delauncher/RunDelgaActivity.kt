package ch.dragondreams.delauncher

import android.content.ContentResolver
import android.content.Intent
import android.graphics.Point
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.SurfaceHolder
import androidx.activity.addCallback
import ch.dragondreams.delauncher.launcher.CustomEvent
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
            when(launcher.state) {
                DragengineLauncher.State.InstallEngineFailed -> activity.onEngineFailed()
                DragengineLauncher.State.LoadLibrariesFailed -> activity.onEngineFailed()
                DragengineLauncher.State.CreateInternalLauncherFailed -> activity.onEngineFailed()
                DragengineLauncher.State.EngineReady -> activity.onEngineReady()
                DragengineLauncher.State.Cancelled -> activity.onEngineFailed()
                else -> {}
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
        EngineDown,
        InitEngine,
        RunGame,
        GameStopped,
        ShutDownEngine
    }

    private val shared = RunGameShared(TAG)
    private var state = State.EngineDown
    private var delgaGames: MutableList<Game> = mutableListOf()
    private var delgaPath: String? = null
    private var runGameHandler: RunGameHandler? = null
    @Volatile
    private var isDestroyingView = false
    private var isResumed = false
    private var isSurfaceReady = false
    private var surfaceHolder: SurfaceHolder? = null
    private val handler = Handler(Looper.getMainLooper())

    override fun getLauncher(): DragengineLauncher {
        if (shared.launcher == null) {
            shared.launcher = DragengineLauncher(this, mSurfaceView.holder.surface)
            //shared.launcher = DragengineLauncher(this, surfaceHolder?.surface)
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
                    shared.logInfo("RunDelgaActivity",
                        "onCreate.mSurfaceView.surfaceCreated: " +
                        "size=${mSurfaceView.width}x${mSurfaceView.height}")
                    shared.surfaceSize = Point(mSurfaceView.width, mSurfaceView.height)
                }

                override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int){
                    shared.logInfo("RunDelgaActivity",
                        "onCreate.mSurfaceView.surfaceChanged: size=${width}x${height}")
                    shared.surfaceSize = Point(width, height)
                    isSurfaceReady = true
                    initEngine()
                }

                override fun surfaceDestroyed(holder: SurfaceHolder){
                    shared.logInfo("RunDelgaActivity",
                        "onCreate.mSurfaceView.surfaceDestroyed")
                    isSurfaceReady = false
                    shutDownEngine()
                }
            })

        } else {
            finish()
        }
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        super.surfaceCreated(holder)

        val r = holder.surfaceFrame
        shared.logInfo("RunDelgaActivity",
            "surfaceCreated: mDestroyed=${mDestroyed} size=${r.width()}x${r.height()}")

        if (!mDestroyed) {
            surfaceHolder = holder
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        super.surfaceChanged(holder, format, width, height)

        val r = holder.surfaceFrame
        shared.logInfo("RunDelgaActivity",
            "surfaceChanged: mDestroyed=${mDestroyed} size=${r.width()}x${r.height()}")
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        shared.logInfo("RunDelgaActivity",
            "surfaceDestroyed: mDestroyed=${mDestroyed}")

        if (!mDestroyed) {
            surfaceHolder = null
        }

        super.surfaceDestroyed(holder)
    }

    override fun onResume() {
        super.onResume()
        shared.logInfo("RunDelgaActivity",
            "onResume: mDestroyed=${mDestroyed}")

        if (!mDestroyed) {
            isResumed = true
            initEngine()
        }
    }

    override fun onPause() {
        super.onPause()
        shared.logInfo("RunDelgaActivity",
            "onPause: mDestroyed=${mDestroyed}")

        if (!mDestroyed) {
            isResumed = false
            shutDownEngine()
        }
    }

    override fun onDestroy() {
        shared.logInfo("RunDelgaActivity", "onDestroy")
        isDestroyingView = true
        shutDownEngine()

        GameActivityAdapter().setHandler(0L)
        runGameHandler?.dispose()
        runGameHandler = null

        delgaGames.forEach { g -> g.release() }
        delgaGames.clear()

        shared.dispose()
        super.onDestroy()
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        shared.logInfo("RunDelgaActivity",
            "onWindowFocusChanged: mDestroyed=${mDestroyed} hasFocus=${hasFocus} " +
                    "isResumed=${isResumed} isSurfaceReady=${isSurfaceReady} state=${state}")
    }

    private fun initEngine() {
        shared.logInfo("RunDelgaActivity",
            "initEngine: isResumed=${isResumed} isSurfaceReady=${isSurfaceReady} state=${state}")
        if (!isResumed || !isSurfaceReady || state != State.EngineDown) {
            return
        }

        state = State.InitEngine
        val l = getLauncher() // force create launcher if not created already

        if (!DEBuildInfo.runDelgaNoFragment) {
            supportFragmentManager
                .beginTransaction()
                .add(android.R.id.content, FragmentInitEngine())
                .commit()
        }
    }

    private fun shutDownEngine() {
        shared.logInfo("RunDelgaActivity",
            "shutDownEngine: isResumed=${isResumed} isSurfaceReady=${isSurfaceReady} state=${state}")
        stopGameWait()

        if (state == State.GameStopped || state == State.InitEngine) {
            state = State.ShutDownEngine
            shared.launcher?.dispose()
            shared.launcher = null
            state = State.EngineDown
        }
    }

    private fun onEngineReady() {
        shared.logInfo("RunDelgaActivity", "onEngineReady: state=${state}")
        if (state != State.InitEngine) {
            return
        }

        state = State.RunGame
        processIntentLaunchDelga()
    }

    private fun onEngineFailed() {
        shared.logInfo("RunDelgaActivity", "onEngineFailed: state=${state}")
        if (state != State.InitEngine) {
            return
        }

        if (DEBuildInfo.runDelgaNoFragment) {
            state = State.EngineDown
            shutDownEngine()
        }
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

        intent.getStringExtra(EXTRA_OWNER_PACKAGE)?.let { p ->
            shared.game?.let { g ->
                g.customProperties[Game.PROPERTY_OWNER_PACKAGE] = p
                g.storeConfig()
            }
        }

        return true
    }

    private fun startGame() {
        val game = shared.game!!

        // start the game
        shared.logInfo("startGame",
            "Cache application ID = '${game.identifier}'")
        shared.logInfo("startGame",
            "Starting game '${game.title}' using profile '${shared.runParams.gameProfile?.name}'")

        loadLibrary("run_game_handler")
        runGameHandler = RunGameHandler(shared.launcher!!, game,
            shared.runParams, HandlerListener(this))
        GameActivityAdapter().setHandler(runGameHandler!!.nativeHandler)

        runOnUiThread {
            onBackPressedDispatcher.addCallback(this) {
                val event = CustomEvent()
                event.type = CustomEvent.Type.BackButton
                GameActivityAdapter().sendCustomEvent(event.convert())
            }

            UIHelper.enableSystemUIBars(window, false)
        }
    }

    private fun stopGameWait() {
        shared.logInfo(TAG, "stopGameWait")
        if (state != State.RunGame) {
            return
        }

        runGameHandler?.stopGame()
        runGameHandler?.waitForState(RunGameHandler.State.GameStopped)
        state = State.GameStopped
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
