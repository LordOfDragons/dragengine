package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.internal.GameActivityAdapter
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
    }

    private val shared = RunGameShared(TAG)
    private var fragmentMain: FragmentRemoteLauncher? = null

    override fun getLauncher(): DragengineLauncher {
        if (shared.launcher == null) {
            shared.launcher = DragengineLauncher(this, mSurfaceView)
            shared.launcher?.logFilename = "deremotelauncher"
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
                fragmentMain?.stopApplicationWait()
            }
        })
    }

    override fun onDestroy() {
        GameActivityAdapter().setHandler(0L)

        shared.dispose()
        super.onDestroy()
    }

    private fun onEngineReady() {
        if (fragmentMain?.engineReady == true) {
            return
        }

        loadLibrary("remote_launcher_handler")
        fragmentMain?.engineReady = true
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
