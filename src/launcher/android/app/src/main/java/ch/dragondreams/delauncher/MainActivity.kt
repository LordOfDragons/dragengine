package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import com.google.android.material.tabs.TabLayout
import androidx.viewpager.widget.ViewPager
import androidx.appcompat.app.AppCompatActivity
import ch.dragondreams.delauncher.ui.main.SectionsPagerAdapter
import ch.dragondreams.delauncher.databinding.ActivityMainBinding
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import kotlin.math.roundToInt

class MainActivity : AppCompatActivity(), InitEngineFragment.Interface {
    class TestListener : DragengineLauncher.DefaultListener() {
        private val tag: String = "MainActivity.TestListener"

        override fun stateChanged(launcher: DragengineLauncher) {
            Log.i(tag, "stateChanged: " + launcher.state)
        }
    }

    private val tag: String = "MainActivity"

    private lateinit var binding: ActivityMainBinding
    private lateinit var launcher: DragengineLauncher

    override fun getLauncher(): DragengineLauncher {
        return launcher
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        launcher = DragengineLauncher(this)
        launcher.addListener(TestListener())

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val sectionsPagerAdapter = SectionsPagerAdapter(this, supportFragmentManager)
        val viewPager: ViewPager = binding.viewPager
        viewPager.adapter = sectionsPagerAdapter
        val tabs: TabLayout = binding.tabs
        tabs.setupWithViewPager(viewPager)
    }
}