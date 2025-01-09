package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.Game
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

    private val shared = RunGameShared(TAG)
    private var state = State.InitEngine
    private var delgaGames: MutableList<Game> = mutableListOf()
    private var delgaPath: String? = null
    private var runGameHandler: RunGameHandler? = null

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
        /*
        enableEdgeToEdge()
        setContentView(R.layout.activity_run_delga)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
        */

        shared.logInfo("RunDelgaActivity", listOf(
            "action='${intent.action}'",
            "type='${intent.type}'",
            "scheme='${intent.scheme}'",
            "data='${intent.data}'").joinToString(" "))

        if (intent.action == "ch.dragondreams.delauncher.LAUNCH_DELGA"
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
                }
            })

        } else {
            finish()
        }
    }

    override fun onDestroy() {
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

        if(!shared.locateProfile()){
            return
        }

        startGame()
    }

    private fun locateGame(): Boolean {
        when (intent.data?.scheme) {
            "content" -> {
                contentResolver.openAssetFileDescriptor(intent.data!!, "r").use { afd ->
                    if (afd == null) {
                        shared.logError("processIntentLaunchDelga",
                            "AssetFileDescriptor is null")
                        return false
                    }

                    val startOffset = afd.startOffset
                    val length = afd.length
                    shared.logInfo("processIntentLaunchDelga",
                        "startOffset=${afd.startOffset} length=${afd.length}"
                    )

                    val fd = afd.parcelFileDescriptor?.detachFd()
                    if (fd == null) {
                        shared.logError("processIntentLaunchDelga",
                            "Can not detach file descriptor")
                        return false
                    }

                    try {
                        shared.launcher?.vfsContainerAddFd(
                            "/$VFS_FDS_DELGA_FILENAME",
                            fd,
                            startOffset.toInt(),
                            length.toInt()
                        )
                    } catch (e: Exception) {
                        shared.logError("processIntentLaunchDelga",
                            "Failed adding VFS container", e)
                        return false
                    }

                    shared.logInfo("processIntentLaunchDelga",
                        "VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            "file" -> {
                contentResolver.openFileDescriptor(intent.data!!, "r").use { pfd ->
                    if (pfd == null) {
                        shared.logError("processIntentLaunchDelga",
                            "ParcelFileDescriptor is null")
                        return false
                    }

                    val length = pfd.statSize
                    shared.logError("processIntentLaunchDelga", "length=${length}")

                    val fd = pfd.detachFd()

                    try {
                        shared.launcher?.vfsContainerAddFd(
                            "/$VFS_FDS_DELGA_FILENAME",
                            fd,
                            0,
                            length.toInt()
                        )
                    } catch (e: Exception) {
                        shared.logError("processIntentLaunchDelga",
                            "Failed adding VFS container", e)
                        return false
                    }

                    shared.logInfo("processIntentLaunchDelga", "VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            else -> {
                shared.logError("processIntentLaunchDelga", "Unsupported scheme")
                return false
            }
        }

        if (delgaPath == null) {
            shared.logError("processIntentLaunchDelga", "DELGA path is null")
            return false
        }

        delgaGames.forEach { g -> g.release() }
        delgaGames.clear()
        delgaGames.addAll(shared.launcher!!.readDelgaGames(delgaPath!!))
        shared.logInfo("processIntentLaunchDelga", "DELGA loaded")

        if (delgaGames.isEmpty()) {
            shared.logError("processIntentLaunchDelga",
                "No game definition found in DELGA file")
            return false
        }

        shared.game = delgaGames[0]

        delgaGames.forEach { g -> g.release() }
        delgaGames.clear()

        shared.loadGameConfig()
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
        runGameHandler = RunGameHandler(shared.launcher!!, game, shared.runParams)
        GameActivityAdapter().setHandler(runGameHandler!!.nativeHandler)
    }

    companion object {
        const val TAG = "RunDelgaActivity"
        private const val VFS_FDS_PATH = "/fds"
        private const val VFS_FDS_DELGA_FILENAME = "game.delga"
        private const val VFS_FDS_DELGA_PATH = "$VFS_FDS_PATH/$VFS_FDS_DELGA_FILENAME"
    }
}
