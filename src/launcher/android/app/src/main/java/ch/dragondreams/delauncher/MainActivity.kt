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
import kotlin.time.TimeSource

class MainActivity : AppCompatActivity() {

    private val TAG: String = "MainActivity"

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val sectionsPagerAdapter = SectionsPagerAdapter(this, supportFragmentManager)
        val viewPager: ViewPager = binding.viewPager
        viewPager.adapter = sectionsPagerAdapter
        val tabs: TabLayout = binding.tabs
        tabs.setupWithViewPager(viewPager)

        // testing
        try {
            Log.i(TAG, "onCreate: Test create launcher")
            val timer = TimeSource.Monotonic.markNow()
            val dl: DragengineLauncher = DragengineLauncher(this)
            Log.i(TAG, "onCreate: Test create launcher finished in "
                    + timer.elapsedNow().inWholeMilliseconds + "ms")
        }catch (e: Exception) {
            Log.e(TAG, "onCreate: Test create launcher", e)
        }
    }
}