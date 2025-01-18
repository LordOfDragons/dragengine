package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.preference.PreferenceManager
import ch.dragondreams.delauncher.databinding.ActivityMainBinding
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.ui.AdapterMainView
import ch.dragondreams.delauncher.ui.FragmentInitEngine
import ch.dragondreams.delauncher.ui.engine.FragmentEngine
import ch.dragondreams.delauncher.ui.game.FragmentGames
import ch.dragondreams.delauncher.ui.settings.FragmentSettings

class MainActivity : AppCompatActivity(),
    FragmentInitEngine.Interface,
    FragmentEngine.Interface,
    FragmentGames.Interface,
    PermissionRequestor.Interface
{
    class LauncherListener : DragengineLauncher.DefaultListener() {
        override fun stateChanged(launcher: DragengineLauncher) {
            Log.i(TAG, "stateChanged: " + launcher.state)
        }

        companion object {
            private const val TAG: String = "MainActivity.TestListener"
        }
    }

    private lateinit var binding: ActivityMainBinding
    private var launcher: DragengineLauncher? = null
    private val permissionRequestor: PermissionRequestor = PermissionRequestor()

    override fun getLauncher(): DragengineLauncher {
        if (launcher == null) {
            launcher = DragengineLauncher(this, null)
            launcher?.logFilename = "delauncher"
            launcher?.addListener(LauncherListener())
            launcher?.initLauncher()
        }
        return launcher!!
    }

    override fun getPermissionRequestor(): PermissionRequestor {
        return permissionRequestor
    }

    private fun isRemoteLauncherEnabled(): Boolean{
        return PreferenceManager.getDefaultSharedPreferences(this).
            getBoolean(FragmentSettings.KEY_REMOTE_LAUNCHER, false)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        getLauncher() // force create launcher if not created already

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val viewPagerAdapter = AdapterMainView(this, supportFragmentManager)
        val viewPager = binding.pagerMain
        viewPager.adapter = viewPagerAdapter
        viewPager.setOffscreenPageLimit(2)
        binding.tabs.setupWithViewPager(viewPager)
    }

    override fun onDestroy() {
        launcher?.dispose()
        launcher = null

        super.onDestroy()
    }

    override fun onRequestPermissionsResult(requestCode: Int,
        permissions: Array<out String>, grantResults: IntArray
    ) {
        if (!permissionRequestor.onRequestPermissionsResult(requestCode, permissions, grantResults)){
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    companion object {
        private val TAG: String = "MainActivity"
    }
}