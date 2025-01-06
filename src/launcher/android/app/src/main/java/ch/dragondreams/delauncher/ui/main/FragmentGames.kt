package ch.dragondreams.delauncher.ui.main

import android.content.ActivityNotFoundException
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.util.Log
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.core.content.FileProvider
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.RunDelgaActivity
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule
import ch.dragondreams.delauncher.launcher.Game
import ch.dragondreams.delauncher.ui.main.FragmentEngine.Interface
import ch.dragondreams.delauncher.ui.main.FragmentEngine.ListenChanges
import java.io.File

/**
 * A fragment representing a list of Items.
 */
class FragmentGames : Fragment() {
    interface Interface {
        fun getLauncher(): DragengineLauncher
    }

    class ListenChanges(
        private val fragment: FragmentGames
    ) : DragengineLauncher.DefaultListener() {
        override fun engineModulesChanged(launcher: DragengineLauncher) {
            fragment.activity?.runOnUiThread {
                fragment.updateViewListGamesData()
            }
        }
    }

    class ListListener(
        private val owner: FragmentGames
    ) : GamesRecyclerViewAdapter.Listener {
        override fun onItemClicked(game: Game) {
            //FragmentEngineModuleInfo(module).show(owner.childFragmentManager, FragmentEngineModuleInfo.TAG)
        }
    }


    private var launcher: DragengineLauncher? = null
    private var viewListGames: RecyclerView? = null
    private var viewListGamesData: MutableList<Game> = mutableListOf()
    private var listenChanges: ListenChanges? = null

    fun updateViewListGamesData() {
        viewListGamesData.clear()
        if(launcher != null){
            viewListGamesData.addAll(launcher!!.games.sortedWith(Comparator { a, b ->
                a.title.compareTo(b.title)
            }))
        }
        viewListGames?.adapter?.notifyDataSetChanged()
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
        val view = inflater.inflate(R.layout.fragment_games, container, false)

        viewListGames = view.findViewById(R.id.listGames)
        with(viewListGames!!) {
            layoutManager = LinearLayoutManager(context)
            adapter = GamesRecyclerViewAdapter(viewListGamesData, ListListener(this@FragmentGames))
        }

        view.findViewById<Button>(R.id.buttonTest).setOnClickListener {
            Log.i(TAG, "onCreateView: ")

            val basePath = File(requireContext().filesDir, "test")
            //val pathDelga = File(basePath, "ddtestproduct.delga")
            //val pathDelga = File(basePath, "DEAudioTest.delga")
            //val pathDelga = File(basePath, "DEExampleApp.delga")
            //val pathDelga = File(basePath, "DEVideoPlayer.delga")
            val pathDelga = File(basePath, "DSTestProject.delga")

            val intent = Intent()
            intent.action = "ch.dragondreams.delauncher.LAUNCH_DELGA"
            intent.setDataAndType(FileProvider.getUriForFile(
                requireContext().applicationContext,
                requireContext().applicationContext.packageName + ".provider",
                pathDelga), "application/dragengine-delga")
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)

            Log.i(TAG, "onCreateView: intent '${intent.action}' '${intent.categories}' '${intent.data}'")

            try {
                startActivity(intent)
            } catch (e: ActivityNotFoundException) {
                Log.e(TAG, "onCreateView: Start activity failed: ${e.message}")
            }
        }
        return view
    }

    companion object {
        const val TAG = "FragmentGames"
    }
}