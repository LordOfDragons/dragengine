package ch.dragondreams.delauncher.ui.main

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ProgressBar
import android.widget.TextView
import androidx.fragment.app.Fragment
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import kotlin.math.roundToInt

class FragmentInitEngine : Fragment() {
    interface Interface {
        fun getLauncher(): DragengineLauncher
    }

    private var launcher: DragengineLauncher? = null
    private val handler = Handler(Looper.getMainLooper())
    private var pbProgress: ProgressBar? = null
    private var labProgress: TextView? = null
    private var labState: TextView? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        launcher = (context as Interface).getLauncher()

        handler.post(object: Runnable {
            override fun run() {
                if (!isAdded || launcher == null) {
                    return
                }

                when(launcher?.state ?: DragengineLauncher.State.EngineReady) {
                    DragengineLauncher.State.InstallEngine -> {
                        labState?.text = resources.getString(R.string.lab_install_update_dragengine)
                        val progress = ((launcher?.progressInstallEngine ?: 0.0) * 100.0).roundToInt()
                        pbProgress?.progress = progress
                        pbProgress?.visibility = View.VISIBLE
                        labProgress?.text = resources.getString(R.string.lab_install_engine_progress, progress)
                    }
                    DragengineLauncher.State.InstallEngineFailed -> {
                        labState?.text = resources.getString(R.string.lab_install_dragengine_failed)
                        pbProgress?.visibility = View.INVISIBLE
                        labProgress?.text = ""
                    }
                    DragengineLauncher.State.LoadLibrariesFailed -> {
                        labState?.text = resources.getString(R.string.lab_load_libraries_failed)
                        pbProgress?.visibility = View.INVISIBLE
                        labProgress?.text = ""
                    }
                    DragengineLauncher.State.CreateInternalLauncherFailed -> {
                        labState?.text = resources.getString(R.string.lab_create_dragengine_failed)
                        pbProgress?.visibility = View.INVISIBLE
                        labProgress?.text = ""
                    }
                    DragengineLauncher.State.EngineReady -> {
                        labState?.text = resources.getString(R.string.lab_dragengine_ready)
                        pbProgress?.visibility = View.INVISIBLE
                        labProgress?.text = ""

                        val t = parentFragmentManager.beginTransaction()
                        t.hide(this@FragmentInitEngine)
                        t.commit()
                    }
                    else -> {}
                }
                handler.postDelayed(this, 150)
            }
        })
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        handler.removeCallbacksAndMessages(null)
        launcher = null
    }

    override fun onDetach() {
        super.onDetach()
        handler.removeCallbacksAndMessages(null)
        launcher = null
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_init_engine, container, false)

        labState = view.findViewById(R.id.labState)
        labState?.text = resources.getString(R.string.lab_verify_dragengine)

        pbProgress = view.findViewById(R.id.pbInstallEngineProgress)
        pbProgress?.progress = 0
        pbProgress?.visibility = View.INVISIBLE

        labProgress = view.findViewById(R.id.labInstallEngineProgress)
        labProgress?.text = ""

        return view
    }
}