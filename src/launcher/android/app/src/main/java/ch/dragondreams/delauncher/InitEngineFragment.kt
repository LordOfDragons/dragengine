package ch.dragondreams.delauncher

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ProgressBar
import android.widget.TextView
import androidx.fragment.app.Fragment
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import kotlin.math.roundToInt

class InitEngineFragment : Fragment() {
    interface Interface {
        fun getLauncher(): DragengineLauncher
    }

    private var launcher: DragengineLauncher? = null
    private val handler = Handler(Looper.getMainLooper())
    private var pbProgress: ProgressBar? = null
    private var labProgress: TextView? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        launcher = (context as Interface).getLauncher()

        handler.post(object: Runnable {
            override fun run() {
                when(launcher?.state ?: DragengineLauncher.State.EngineReady) {
                    DragengineLauncher.State.InstallEngine -> {
                        val progress = ((launcher?.progressInstallEngine ?: 0.0) * 100.0).roundToInt()
                        pbProgress?.progress = progress
                        labProgress?.text = resources.getString(R.string.lab_install_engine_progress, progress)
                    }
                    DragengineLauncher.State.InstallEngineFailed -> {
                        labProgress?.text = "ERROR Install"
                    }
                    DragengineLauncher.State.LoadLibrariesFailed -> {
                        labProgress?.text = "ERROR Load"
                    }
                    DragengineLauncher.State.EngineReady -> {
                        val t = parentFragmentManager.beginTransaction()
                        t.hide(this@InitEngineFragment)
                        t.commit()
                    }
                    else -> {}
                }
                handler.postDelayed(this, 250)
            }
        })
    }

    override fun onDetach() {
        super.onDetach()
        launcher = null
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_init_engine, container, false)
        pbProgress = view.findViewById(R.id.pbInstallEngineProgress)
        labProgress = view.findViewById(R.id.labInstallEngineProgress)
        return view
    }
}