package ch.dragondreams.delauncher

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup

class FragmentRemoteLauncher : Fragment() {
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_remote_launcher, container, false)
    }

    companion object {
        const val TAG = "FragmentRemoteLauncher"
    }
}