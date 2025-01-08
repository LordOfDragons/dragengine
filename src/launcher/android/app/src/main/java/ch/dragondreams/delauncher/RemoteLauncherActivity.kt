package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.internal.GameActivityAdapter
import ch.dragondreams.delauncher.launcher.internal.RemoteLauncherHandler
import ch.dragondreams.delauncher.ui.main.FragmentInitEngine
import ch.dragondreams.delauncher.ui.main.FragmentRemoteLauncher
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

        override fun launcherCreated(launcher: DragengineLauncher) {
            launcher.launcher?.addFileLogger("deremotelauncher")
        }
    }

    enum class State {
        InitEngine,
        Idle,
        RunGame
    }

    private val shared = RunGameShared(TAG)
    private var state = State.InitEngine
    private var handler: RemoteLauncherHandler? = null
    private var fragmentMain: FragmentRemoteLauncher? = null

    override fun getLauncher(): DragengineLauncher {
        if (shared.launcher == null) {
            shared.launcher = DragengineLauncher(this, mSurfaceView)
            shared.launcher?.addListener(RemoteLauncherListener(this))
            shared.launcher?.initLauncher()
        }
        return shared.launcher!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        loadLibrary("game_activity_adapter")
        super.onCreate(savedInstanceState)

        fragmentMain = FragmentRemoteLauncher()
        fragmentMain?.shared = shared
        supportFragmentManager.beginTransaction()
            .add(android.R.id.content, fragmentMain!!).commit()

        /*
        if (Build.VERSION.SDK_INT >= 30){
            window.insetsController

        } else {
            window.decorView.systemUiVisibility = (
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE or
                            View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    )
        }
        */

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
    }

    override fun onDestroy() {
        GameActivityAdapter().setHandler(0L)
        handler?.dispose()
        handler = null

        shared.dispose()
        super.onDestroy()
    }

    private fun onEngineReady() {
        if (state != State.InitEngine) {
            return
        }
        state = State.Idle

        loadLibrary("remote_launcher_handler")
        fragmentMain?.engineReady = true
    }

    private fun runGame() {

    }

    private fun prepareGame(): Boolean{
        if(shared.game == null){
            return false
        }

        shared.loadGameConfig()
        return true
    }

    private fun startGame() {
        // start the game
        shared.logInfo("startGame",
            "Cache application ID = '${shared.game?.identifier}'")
        shared.logInfo("startGame",
            "Starting game '${shared.game?.title}' using profile '${shared.runParams.gameProfile?.name}'");

        handler = RemoteLauncherHandler(shared.launcher!!)
        GameActivityAdapter().setHandler(handler!!.nativeHandler)
    }

    override fun onRequestPermissionsResult(requestCode: Int,
        permissions: Array<out String>, grantResults: IntArray
    ) {
        when(requestCode) {
            FragmentRemoteLauncher.REQUEST_CODE_PERMISSION_INTERNET ->
                fragmentMain?.onPermissionResult(requestCode, permissions, grantResults)

            else -> super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    companion object {
        const val TAG = "RemoteLauncherActivity"
    }
}
