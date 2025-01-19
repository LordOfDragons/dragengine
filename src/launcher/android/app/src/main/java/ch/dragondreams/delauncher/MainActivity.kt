package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import ch.dragondreams.delauncher.databinding.ActivityMainBinding
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.ui.AdapterMainView
import ch.dragondreams.delauncher.ui.FragmentInitEngine
import ch.dragondreams.delauncher.ui.engine.FragmentEngine
import ch.dragondreams.delauncher.ui.example.Example
import ch.dragondreams.delauncher.ui.game.FragmentGames

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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if(processIntentExtras()) {
            finish()
            return
        }

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

    private fun processIntentExtras(): Boolean {
        val gameId = intent.getStringExtra(RunDelgaActivity.EXTRA_GAME_ID)
        if (gameId != null) {
            return examples.find { e -> e.gameId == gameId }?.run(this) == true
        }

        return false
    }

    companion object {
        private const val TAG: String = "MainActivity"

        private val EXAMPLES_BASE_URL = "https://github.com/LordOfDragons/deexamples/releases/latest/download"

        val examples = listOf(
            Example("UI Example",
                "$EXAMPLES_BASE_URL/DSTestProject.delga",
                "9febef48-f384-4ca2-899e-ae8efdaf996d"),
            Example("3D Example",
                "$EXAMPLES_BASE_URL/DEExampleApp.delga",
                "73ac1719-c9b2-40c4-8cd7-6efa8f5b63f9"),
            Example("Audio Example",
                "$EXAMPLES_BASE_URL/DEAudioTest.delga",
                "355358e9-d3c2-48d3-bbe7-416d45efd9ed")
        )
    }
}