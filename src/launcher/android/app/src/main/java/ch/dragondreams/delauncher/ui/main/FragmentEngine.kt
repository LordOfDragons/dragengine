package ch.dragondreams.delauncher.ui.main

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule

class FragmentEngine : Fragment() {
    interface Interface {
        fun getLauncher(): DragengineLauncher
    }

    class ListenChanges(
        private val fragment: FragmentEngine
    ) : DragengineLauncher.DefaultListener() {
        override fun engineModulesChanged(launcher: DragengineLauncher) {
            fragment.activity?.runOnUiThread {
                fragment.updateViewListModulesData()
            }
        }

        override fun stateChanged(launcher: DragengineLauncher) {
            fragment.activity?.runOnUiThread {
                fragment.updateDragengineInfo()
            }
        }
    }

    class ListListener(
        private val owner: FragmentEngine
    ) : EngineModuleRecyclerViewAdapter.Listener {
        override fun onItemClicked(module: EngineModule) {
            FragmentEngineModuleInfo(module).show(owner.childFragmentManager, FragmentEngineModuleInfo.TAG)
        }
    }


    private var launcher: DragengineLauncher? = null
    private var viewListModules: RecyclerView? = null
    private var viewListModulesData: MutableList<EngineModule> = mutableListOf()
    private var listenChanges: ListenChanges? = null

    fun updateViewListModulesData() {
        viewListModulesData.clear()
        if(launcher != null) {
            viewListModulesData.addAll(launcher!!.engineModules.sortedWith(Comparator { a, b ->
                if (a.status == EngineModule.Status.Ready) {
                    if (b.status == EngineModule.Status.Ready) {
                        a.name.compareTo(b.name)
                    } else {
                        1
                    }
                } else {
                    if (b.status == EngineModule.Status.Ready) {
                        -1
                    } else {
                        a.name.compareTo(b.name)
                    }
                }
            }))
        }
        viewListModules?.adapter?.notifyDataSetChanged()
    }

    private fun updateDragengineInfo() {
        view?.findViewById<TextView>(R.id.textDragengineVersion)?.text = launcher!!.engineVersion
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        launcher = (context as Interface).getLauncher()
        listenChanges = ListenChanges(this)
        launcher!!.addListener(listenChanges!!)
    }

    override fun onDetach() {
        launcher?.removeListener(listenChanges!!)
        super.onDetach()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_engine, container, false)
        view.findViewById<TextView>(R.id.textDragengineVersion).text = launcher!!.engineVersion
        viewListModules = view.findViewById(R.id.listEngineModules)
        with(viewListModules!!) {
            layoutManager = LinearLayoutManager(context)
            adapter = EngineModuleRecyclerViewAdapter(viewListModulesData, ListListener(this@FragmentEngine))
        }
        return view
    }
}