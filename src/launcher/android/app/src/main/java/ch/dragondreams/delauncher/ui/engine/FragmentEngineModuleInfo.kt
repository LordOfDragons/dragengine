package ch.dragondreams.delauncher.ui.engine

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Switch
import android.widget.TextView
import androidx.fragment.app.DialogFragment
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.launcher.EngineModule


class FragmentEngineModuleInfo(
    val module: EngineModule
) : DialogFragment() {

    override fun onStart() {
        super.onStart()
        dialog?.window?.let { w ->
            val width = ViewGroup.LayoutParams.MATCH_PARENT
            val height = ViewGroup.LayoutParams.MATCH_PARENT
            w.setLayout(width, height)
        }
    }

    /*
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setStyle(DialogFragment.STYLE_NORMAL,
            android.R.style.Theme_Black_NoTitleBar_Fullscreen);
    }
    */

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_engine_module_info, container, false)
        view.findViewById<TextView>(R.id.textName).text = module.name
        view.findViewById<TextView>(R.id.textType).text = EngineModule.mapTypeName[module.type]
        view.findViewById<TextView>(R.id.textDescription).text = module.description
        view.findViewById<TextView>(R.id.textAuthor).text = module.author
        view.findViewById<TextView>(R.id.textVersion).text = module.version
        view.findViewById<TextView>(R.id.textPattern).text = module.pattern
        view.findViewById<TextView>(R.id.textPriority).text = "${module.priority}"
        view.findViewById<Switch>(R.id.switchFallback).isChecked = module.isFallback
        view.findViewById<TextView>(R.id.textStatus).text = EngineModule.mapStatusName[module.status]
        view.findViewById<TextView>(R.id.textErrorCode).text = "${module.errorCode}"
        view.findViewById<TextView>(R.id.textLibFileSize).text = "${module.libFileSizeIs}"
        view.findViewById<TextView>(R.id.textLibFileHash).text = module.libFileHashIs

        view.findViewById<Button>(R.id.buttonClose).setOnClickListener { dismiss() }
        return view
    }

    companion object {
        const val TAG = "FragmentEngineModuleInfo"
    }
}