package ch.dragondreams.delauncher

import android.graphics.Point
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.internal.GameActivityAdapter
import ch.dragondreams.delauncher.ui.FragmentInitEngine
import ch.dragondreams.delauncher.ui.remote.FragmentRemoteLauncher
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
    private val permissionRequestor: PermissionRequestor = PermissionRequestor()

    override fun getLauncher(): DragengineLauncher {
        if (shared.launcher == null) {
            shared.launcher = DragengineLauncher(this, mSurfaceView.holder.surface)
            shared.launcher?.logFilename = "deremotelauncher"
            shared.launcher?.addListener(RemoteLauncherListener(this))
            shared.launcher?.initLauncher()
        }
        return shared.launcher!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        loadLibrary("game_activity_adapter")
        super.onCreate(savedInstanceState)

        fragmentMain = FragmentRemoteLauncher(permissionRequestor)
        fragmentMain?.shared = shared
        supportFragmentManager.beginTransaction()
            .add(android.R.id.content, fragmentMain!!).commit()

        mSurfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(holder: SurfaceHolder){
                shared.logInfo("RemoteLauncherActivity", listOf(
                    "surfaceView created:",
                    "size=${mSurfaceView.width}x${mSurfaceView.height}").
                        joinToString(" "))
                shared.surfaceSize = Point(mSurfaceView.width, mSurfaceView.height)
                getLauncher() // force create launcher if not created already
                supportFragmentManager.beginTransaction()
                    .add(android.R.id.content, FragmentInitEngine()).commit()
            }

            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int){
                shared.logInfo("RunDelgaActivity", listOf(
                    "surfaceView changed:",
                    "size=${width}x${height}").
                        joinToString(" "))
                shared.surfaceSize = Point(width, height)
            }

            override fun surfaceDestroyed(holder: SurfaceHolder){
                shared.logInfo("RunDelgaActivity", "surfaceView destroyed")
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
        if (!permissionRequestor.onRequestPermissionsResult(requestCode, permissions, grantResults)){
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    companion object {
        const val TAG = "RemoteLauncherActivity"
    }
}
