package ch.dragondreams.delauncher.ui.main

import android.content.Context
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentStatePagerAdapter
import ch.dragondreams.delauncher.R

class MainViewPagesAdapter(private val context: Context, fm: FragmentManager) :
    FragmentStatePagerAdapter(fm) {

    var showRemoteLauncher = false
        set(value) {
            if(value == field){
                return
            }

            field = value

            keys.clear()
            if(value){
                keys.addAll(listOf(KEY_GAMES, KEY_ENGINE, KEY_REMOTE_LAUNCHER, KEY_SETTINGS))
            }else{
                keys.addAll(listOf(KEY_GAMES, KEY_ENGINE, KEY_SETTINGS))
            }

            notifyDataSetChanged()
        }

    private val fragments: MutableMap<String,Fragment> = mutableMapOf()
    private val keys: MutableList<String> = mutableListOf(KEY_GAMES, KEY_ENGINE, KEY_SETTINGS)

    override fun getItem(position: Int): Fragment {
        val key = keys[position]
        var fragment = fragments[key]
        if(fragment == null) {
            fragment = when (key) {
                KEY_GAMES -> FragmentGames()
                KEY_ENGINE -> FragmentEngine()
                KEY_REMOTE_LAUNCHER -> FragmentRemoteLauncher()
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

    override fun getItemPosition(page: Any): Int {
        return POSITION_NONE
    }

    fun getKeyAt(index: Int): String{
        return keys[index]
    }

    fun getKeyPosition(key: String): Int{
        return keys.indexOf(key)
    }

    companion object{
        const val KEY_GAMES = "games"
        const val KEY_ENGINE = "engine"
        const val KEY_REMOTE_LAUNCHER = "remote_launcher"
        const val KEY_SETTINGS = "settings"

        val TAB_NAMES = mapOf(
            KEY_GAMES to R.string.tab_games,
            KEY_ENGINE to R.string.tab_engine,
            KEY_REMOTE_LAUNCHER to R.string.tab_remote_launcher,
            KEY_SETTINGS to R.string.tab_settings
        )
    }
}