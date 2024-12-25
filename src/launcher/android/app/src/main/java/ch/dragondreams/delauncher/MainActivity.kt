package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import com.google.android.material.tabs.TabLayout
import androidx.viewpager.widget.ViewPager
import androidx.appcompat.app.AppCompatActivity
import ch.dragondreams.delauncher.ui.main.SectionsPagerAdapter
import ch.dragondreams.delauncher.databinding.ActivityMainBinding
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.ui.main.FragmentInitEngine

class MainActivity : AppCompatActivity(), FragmentInitEngine.Interface {
    class TestListener : DragengineLauncher.DefaultListener() {
        private val tag: String = "MainActivity.TestListener"

        override fun stateChanged(launcher: DragengineLauncher) {
            Log.i(tag, "stateChanged: " + launcher.state)
        }
    }

    private val tag: String = "MainActivity"

    private lateinit var binding: ActivityMainBinding
    private var launcher: DragengineLauncher? = null

    override fun getLauncher(): DragengineLauncher {
        if (launcher == null) {
            launcher = DragengineLauncher(this)
            launcher!!.addListener(TestListener())
        }
        return launcher!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        getLauncher() // force create launcher if not created already

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val sectionsPagerAdapter = SectionsPagerAdapter(this, supportFragmentManager)
        val viewPager: ViewPager = binding.viewPager
        viewPager.adapter = sectionsPagerAdapter
        val tabs: TabLayout = binding.tabs
        tabs.setupWithViewPager(viewPager)
    }

    override fun onDestroy() {
        launcher?.dispose()
        launcher = null

        super.onDestroy()
    }
}