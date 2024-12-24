package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.snackbar.Snackbar
import com.google.android.material.tabs.TabLayout
import androidx.viewpager.widget.ViewPager
import androidx.appcompat.app.AppCompatActivity
import android.view.Menu
import android.view.MenuItem
import ch.dragondreams.delauncher.ui.main.SectionsPagerAdapter
import ch.dragondreams.delauncher.databinding.ActivityMainBinding
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.launch
import kotlin.math.floor
import kotlin.math.roundToInt
import kotlin.time.TimeSource

class MainActivity : AppCompatActivity() {
    class TestListener : DragengineLauncher.DefaultListener() {
        private val TAG: String = "MainActivity.TestListener"

        override fun stateChanged(launcher: DragengineLauncher) {
            Log.i(TAG, "stateChanged: " + launcher.state)
        }
    }

    private val TAG: String = "MainActivity"

    private lateinit var binding: ActivityMainBinding
    private lateinit var launcher: DragengineLauncher

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        launcher = DragengineLauncher(this)
        launcher.addListener(TestListener())
        GlobalScope.launch{ testCheckProgress() }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val sectionsPagerAdapter = SectionsPagerAdapter(this, supportFragmentManager)
        val viewPager: ViewPager = binding.viewPager
        viewPager.adapter = sectionsPagerAdapter
        val tabs: TabLayout = binding.tabs
        tabs.setupWithViewPager(viewPager)
    }

    private fun testCheckProgress() {
        while (true) {
            when(launcher.state) {
                DragengineLauncher.State.InstallEngine -> {
                    Log.i(TAG, "testCheckProgress: " + (launcher.progressInstallEngine * 100.0).roundToInt() + "%")
                }
                DragengineLauncher.State.InstallEngineFailed -> return
                DragengineLauncher.State.LoadLibrariesFailed -> return
                DragengineLauncher.State.EngineReady -> return
                else -> {}
            }
            Thread.sleep(250)
        }
    }
}