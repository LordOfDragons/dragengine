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
    class TestListener : DragengineLauncher.DefaultListener() {
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
            launcher!!.addListener(TestListener())
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
        viewPagerAdapter.showRemoteLauncher = isRemoteLauncherEnabled()
        val viewPager = binding.pagerMain
        viewPager.adapter = viewPagerAdapter
        viewPager.setOffscreenPageLimit(3)
        val tabs = binding.tabs
        tabs.setupWithViewPager(viewPager)

        PreferenceManager.getDefaultSharedPreferences(this).
            registerOnSharedPreferenceChangeListener { _, key ->
                if (key == FragmentSettings.KEY_REMOTE_LAUNCHER){
                    runOnUiThread {
                        val key = viewPagerAdapter.getKeyAt(tabs.selectedTabPosition)

                        viewPagerAdapter.showRemoteLauncher = isRemoteLauncherEnabled()
                        for (i in 0..<viewPagerAdapter.count) {
                            viewPagerAdapter.instantiateItem(viewPager, i)
                        }

                        val position = viewPagerAdapter.getKeyPosition(key)
                        if(position != -1) {
                            tabs.selectTab(tabs.getTabAt(position))
                        }
                    }
                }
            }
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