package ch.dragondreams.delauncher.ui.main

import android.content.Intent
import android.os.Bundle
import android.util.Log
import androidx.preference.Preference
import androidx.preference.PreferenceFragmentCompat
import androidx.preference.SwitchPreferenceCompat
import androidx.viewpager.widget.ViewPager
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.RemoteLauncherActivity
import ch.dragondreams.delauncher.UIHelper


class FragmentSettings : PreferenceFragmentCompat() {
    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.preferences, rootKey)

        findPreference<Preference>(KEY_PROFILES)?.setOnPreferenceClickListener {
            Log.i(TAG, "onCreatePreferences: Profiles clicked")
            return@setOnPreferenceClickListener true
        }

        findPreference<Preference>(KEY_SUPPORT_DISCORD)?.setOnPreferenceClickListener {
            UIHelper.openUrl(requireActivity(), "https://discord.gg/Jeg62ns")
            return@setOnPreferenceClickListener true
        }

        findPreference<Preference>(KEY_SUPPORT_WEBSITE)?.setOnPreferenceClickListener {
            UIHelper.openUrl(requireActivity(), "https://dragondreams.ch/index.php/about-us")
            return@setOnPreferenceClickListener true
        }

        findPreference<Preference>(KEY_REMOTE_LAUNCHER)?.setOnPreferenceClickListener {
            startActivity(Intent(requireContext(), RemoteLauncherActivity::class.java))
            return@setOnPreferenceClickListener true
        }
    }

    companion object{
        const val TAG = "SettingsFragment"

        const val KEY_PROFILES = "profiles"
        const val KEY_SUPPORT_DISCORD = "support_discord"
        const val KEY_SUPPORT_WEBSITE = "support_website"
        const val KEY_REMOTE_LAUNCHER = "remote_launcher"
    }
}
