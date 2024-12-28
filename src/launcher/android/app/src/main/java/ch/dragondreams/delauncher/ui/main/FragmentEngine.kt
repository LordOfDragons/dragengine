package ch.dragondreams.delauncher.ui.main

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
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
    }

    private var launcher: DragengineLauncher? = null
    private var viewListModules: RecyclerView? = null
    private var viewListModulesData: MutableList<EngineModule> = ArrayList()
    private var listenChanges: ListenChanges? = null

    fun updateViewListModulesData() {
        viewListModulesData.clear()
        if(launcher != null) {
            viewListModulesData.addAll(launcher!!.engineModules.sortedWith(Comparator { a, b ->
                a.name.compareTo(b.name)
            }))
        }
        viewListModules?.adapter?.notifyDataSetChanged()
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
        viewListModules = view.findViewById<RecyclerView>(R.id.listEngineModules)
        with(viewListModules!!) {
            layoutManager = LinearLayoutManager(context)
            adapter = EngineModuleRecyclerViewAdapter(viewListModulesData)
        }
        return view
    }

    companion object {
        @JvmStatic
        fun newInstance() =
            FragmentEngine().apply {
                arguments = Bundle().apply {
                }
            }
    }
}