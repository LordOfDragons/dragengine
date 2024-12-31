package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.Game
import ch.dragondreams.delauncher.ui.main.FragmentInitEngine

class RunDelgaActivity : AppCompatActivity(),
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

    override fun getLauncher(): DragengineLauncher {
        if (launcher == null) {
            launcher = DragengineLauncher(this)
            launcher!!.addListener(RunLauncherListener(this))
        }
        return launcher!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_run_delga)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        Log.i(TAG, listOf("RunDelgaActivity:",
            "action='${intent.action}'",
            "type='${intent.type}'",
            "scheme='${intent.scheme}'",
            "data='${intent.data}'").joinToString(" "))

        if (intent.action == "ch.dragondreams.delauncher.LAUNCH_DELGA"
            || intent.action == "android.intent.action.VIEW") {
            getLauncher() // force create launcher if not created already

        } else {
            finish()
        }
    }

    override fun onDestroy() {
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

        if (!locateGame()) {
            finish()
            return
        }

        delgaGame?.dispose()
    }

    private fun locateGame(): Boolean {
        when (intent.data?.scheme) {
            "content" -> {
                contentResolver.openAssetFileDescriptor(intent.data!!, "r").use { afd ->
                    if (afd == null) {
                        Log.e(TAG, "processIntentLaunchDelga: AssetFileDescriptor is null")
                        return false
                    }

                    val startOffset = afd.startOffset
                    val length = afd.length
                    Log.e(TAG,
                        "processIntentLaunchDelga: startOffset=${afd.startOffset} length=${afd.length}"
                    )

                    val fd = afd.parcelFileDescriptor?.detachFd()
                    if (fd == null) {
                        Log.e(TAG, "processIntentLaunchDelga: Can not detach file descriptor")
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
                        Log.e(TAG, "processIntentLaunchDelga: Failed adding VFS container", e)
                        return false
                    }

                    Log.i(TAG, "processIntentLaunchDelga: VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            "file" -> {
                contentResolver.openFileDescriptor(intent.data!!, "r").use { pfd ->
                    if (pfd == null) {
                        Log.e(TAG, "processIntentLaunchDelga: ParcelFileDescriptor is null")
                        return false
                    }

                    val length = pfd.statSize
                    Log.e(TAG, "processIntentLaunchDelga: length=${length}")

                    val fd = pfd.detachFd()

                    try {
                        launcher!!.vfsContainerAddFd(
                            "/$VFS_FDS_DELGA_FILENAME",
                            fd,
                            0,
                            length.toInt()
                        )
                    } catch (e: Exception) {
                        Log.e(TAG, "processIntentLaunchDelga: Failed adding VFS container", e)
                        return false
                    }

                    Log.i(TAG, "processIntentLaunchDelga: VFS container added")
                    delgaPath = VFS_FDS_DELGA_PATH
                }
            }

            else -> {
                Log.e(TAG, "processIntentLaunchDelga: Unsupported scheme")
                return false
            }
        }

        if (delgaPath == null) {
            Log.e(TAG, "processIntentLaunchDelga: DELGA path is null")
            return false
        }

        delgaGames.forEach { g -> g.dispose() }
        delgaGames.clear()
        delgaGames.addAll(launcher!!.readDelgaGames(delgaPath!!))
        Log.i(TAG, "processIntentLaunchDelga: DELGA loaded")

        if (delgaGames.isEmpty()) {
            Log.e(TAG, "processIntentLaunchDelga: No game definition found in DELGA file")
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

        delgaGame?.verifyRequirements()
        delgaGame?.updateStatus()
        Log.i(TAG, "processIntentLaunchDelga: DELGA status ${delgaGame?.canRun}")
        return true
    }

    companion object {
        const val TAG = "RunDelgaActivity"
        private const val VFS_FDS_PATH = "/fds"
        private const val VFS_FDS_DELGA_FILENAME = "game.delga"
        private const val VFS_FDS_DELGA_PATH = "$VFS_FDS_PATH/$VFS_FDS_DELGA_FILENAME"
    }
}
