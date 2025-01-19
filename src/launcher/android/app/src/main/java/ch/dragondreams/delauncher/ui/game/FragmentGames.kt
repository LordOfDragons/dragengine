package ch.dragondreams.delauncher.ui.game

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import ch.dragondreams.delauncher.MainActivity.Companion.examples
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.RunDelgaActivity
import ch.dragondreams.delauncher.UIHelper
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.Game


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
    ) : AdapterGames.Listener {
        override fun onItemClicked(game: Game) {
            Log.i(TAG, "onItemClicked: '${game.identifier}' -> '${game.customProperties[Game.PROPERTY_OWNER_PACKAGE]}'")
            //FragmentEngineModuleInfo(module).show(owner.childFragmentManager, FragmentEngineModuleInfo.TAG)

            val ownerPackage = game.customProperties[Game.PROPERTY_OWNER_PACKAGE]
            if (ownerPackage != null) {
                if (ownerPackage == owner.requireActivity().packageName) {
                    examples.find { e -> e.gameId == game.identifier }?.run(owner.requireActivity())
                    return
                }

                var intent = owner.requireActivity().packageManager.getLaunchIntentForPackage(ownerPackage)
                Log.i(TAG, "intent: $intent")

                if (intent == null) {
                    intent = Intent(Intent.ACTION_MAIN)
                    intent.setClassName(ownerPackage, "${ownerPackage}.MainActivity")
                }

                intent.putExtra(RunDelgaActivity.EXTRA_GAME_ID, game.identifier)
                try {
                    owner.requireActivity().startActivity(intent)
                } catch (e: Exception) {
                    if (e.message != null) {
                        UIHelper.showError(owner.requireActivity(), e.message!!)
                    } else {
                        UIHelper.showError(owner.requireActivity(), R.string.error_example_run)
                    }
                }
            }
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
            adapter = AdapterGames(viewListGamesData, ListListener(this@FragmentGames))
        }

        val viewExamplesTitle = view.findViewById<View>(R.id.viewExamplesTitle)
        val viewExamplesContent = view.findViewById<View>(R.id.viewExamplesContent)
        val imgExamplesDropDown = view.findViewById<ImageView>(R.id.imgExamplesDropDown)

        viewExamplesContent.visibility = View.GONE
        imgExamplesDropDown.animate().rotation(-90f)

        viewExamplesTitle?.setOnClickListener {
            if (viewExamplesContent.isVisible) {
                viewExamplesContent.visibility = View.GONE
                imgExamplesDropDown.animate().rotation(-90f)
            } else {
                viewExamplesContent.visibility = View.VISIBLE
                imgExamplesDropDown.animate().rotation(0f)
            }
        }

        return view
    }

    companion object {
        const val TAG = "FragmentGames"
    }
}