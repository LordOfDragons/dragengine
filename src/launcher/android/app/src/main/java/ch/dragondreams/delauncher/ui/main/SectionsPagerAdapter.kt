package ch.dragondreams.delauncher.ui.main

import android.content.Context
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentPagerAdapter
import ch.dragondreams.delauncher.FragmentGames
import ch.dragondreams.delauncher.R
import kotlinx.coroutines.selects.select

private val TAB_TITLES = arrayOf(
    R.string.tab_games,
    R.string.tab_profiles,
    R.string.tab_logs
)

/**
 * A [FragmentPagerAdapter] that returns a fragment corresponding to
 * one of the sections/tabs/pages.
 */
class SectionsPagerAdapter(private val context: Context, fm: FragmentManager) :
    FragmentPagerAdapter(fm) {

    override fun getItem(position: Int): Fragment {
        return when(position) {
            0 -> FragmentGames.newInstance();
            else -> PlaceholderFragment.newInstance(position + 1)
        }
    }

    override fun getPageTitle(position: Int): CharSequence? {
        return context.resources.getString(TAB_TITLES[position])
    }

    override fun getCount(): Int {
        return 3
    }
}