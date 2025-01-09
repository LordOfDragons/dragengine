package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.preference.PreferenceManager
import ch.dragondreams.delauncher.databinding.ActivityMainBinding
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.ui.main.FragmentEngine
import ch.dragondreams.delauncher.ui.main.FragmentGames
import ch.dragondreams.delauncher.ui.main.FragmentInitEngine
import ch.dragondreams.delauncher.ui.main.FragmentSettings
import ch.dragondreams.delauncher.ui.main.MainViewPagesAdapter

class MainActivity : AppCompatActivity(),
    FragmentInitEngine.Interface,
    FragmentEngine.Interface,
    FragmentGames.Interface
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

    override fun getLauncher(): DragengineLauncher {
        if (launcher == null) {
            launcher = DragengineLauncher(this, null)
            launcher?.logFilename = "delauncher"
            launcher?.addListener(LauncherListener())
            launcher?.initLauncher()
        }
        return launcher!!
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

        val viewPagerAdapter = MainViewPagesAdapter(this, supportFragmentManager)
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

    companion object {
        private val TAG: String = "MainActivity"
    }
}