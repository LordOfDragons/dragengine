package ch.dragondreams.delauncher.ui.main

import android.content.Context
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentStatePagerAdapter
import ch.dragondreams.delauncher.R

class MainViewPagesAdapter(private val context: Context, fm: FragmentManager) :
    FragmentStatePagerAdapter(fm) {

    private val fragments: MutableMap<String,Fragment> = mutableMapOf()
    private val keys: List<String> = listOf(KEY_GAMES, KEY_ENGINE, KEY_SETTINGS)

    override fun getItem(position: Int): Fragment {
        val key = keys[position]
        var fragment = fragments[key]
        if(fragment == null) {
            fragment = when (key) {
                KEY_GAMES -> FragmentGames()
                KEY_ENGINE -> FragmentEngine()
                KEY_SETTINGS -> FragmentSettings()
                else -> throw Exception("unsupported key $key")
            }
            fragments[key] = fragment
        }
        return fragment
    }

    override fun getPageTitle(position: Int): CharSequence {
        return context.resources.getString(TAB_NAMES[keys[position]]!!)
    }

    override fun getCount(): Int {
        return keys.size
    }

    companion object{
        const val KEY_GAMES = "games"
        const val KEY_ENGINE = "engine"
        const val KEY_SETTINGS = "settings"

        val TAB_NAMES = mapOf(
            KEY_GAMES to R.string.tab_games,
            KEY_ENGINE to R.string.tab_engine,
            KEY_SETTINGS to R.string.tab_settings
        )
    }
}