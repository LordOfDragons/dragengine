package ch.dragondreams.delauncher.ui.main

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.core.widget.doAfterTextChanged
import androidx.core.widget.doOnTextChanged
import androidx.preference.PreferenceManager
import ch.dragondreams.delauncher.R

class FragmentRemoteLauncher : Fragment() {
    private var editHostAddress: TextView? = null
    private var editClientName: TextView? = null
    private var buttonConnect: Button? = null
    private var buttonDisconnect: Button? = null

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_remote_launcher, container, false)
        val pref = PreferenceManager.getDefaultSharedPreferences(requireContext())

        editHostAddress = view.findViewById(R.id.editHostAddress)
        editHostAddress?.text = pref.getString(PREF_HOST, "")
        editHostAddress?.doAfterTextChanged { text ->
            val edit = pref.edit()
            edit.putString(PREF_HOST, text.toString())
            edit.apply()
        }

        editClientName = view.findViewById(R.id.editClientName)
        editClientName?.text = pref.getString(PREF_CLIENT_NAME, "Android")
        editClientName?.doAfterTextChanged { text ->
            val edit = pref.edit()
            edit.putString(PREF_CLIENT_NAME, text.toString())
            edit.apply()
        }

        buttonConnect = view.findViewById(R.id.buttonConnect)
        buttonConnect?.setOnClickListener { onButtonConnect() }

        buttonDisconnect = view.findViewById(R.id.buttonDisconnect)
        buttonDisconnect?.setOnClickListener { onButtonDisconnect() }

        updateUIState()
        return view
    }

    private fun onButtonConnect(){

    }

    private fun onButtonDisconnect(){

    }

    private fun updateUIState(){
        editClientName?.isEnabled = true
        editHostAddress?.isEnabled = true
        buttonConnect?.isEnabled = true
        buttonDisconnect?.isEnabled = false
    }

    companion object {
        const val TAG = "FragmentRemoteLauncher"

        const val PREF_HOST = "remote_launcher_host"
        const val PREF_CLIENT_NAME = "remote_launcher_clientname"
    }
}