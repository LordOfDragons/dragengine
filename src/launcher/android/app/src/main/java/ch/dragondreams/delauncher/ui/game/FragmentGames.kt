package ch.dragondreams.delauncher.ui.game

import android.content.ClipData
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.ContextMenu
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import androidx.core.content.FileProvider
import androidx.core.content.pm.PackageInfoCompat
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
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.util.zip.ZipEntry
import java.util.zip.ZipOutputStream


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

        override fun gamesChanged(launcher: DragengineLauncher) {
            fragment.activity?.runOnUiThread {
                fragment.updateViewListGamesData()
            }
        }
    }

    class ListListener(
        private val owner: FragmentGames
    ) : AdapterGames.Listener {
        override fun onItemClicked(game: Game) {
            owner.runGame(game)
        }

        override fun onCreateContextMenu(game: Game, menu: ContextMenu) {
            owner.showGameContextMenu(game, menu)
        }
    }


    private var launcher: DragengineLauncher? = null
    private var viewListGames: RecyclerView? = null
    private var viewListGamesData: MutableList<Game> = mutableListOf()
    private var listenChanges: ListenChanges? = null
    private var contextMenuGame: Game? = null

    fun updateViewListGamesData() {
        viewListGamesData.clear()
        launcher?.let { l ->
            viewListGamesData.addAll(l.games.sortedWith { a, b -> a.title.compareTo(b.title) })
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

    private fun showGameContextMenu(game: Game, menu: ContextMenu) {
        requireActivity().menuInflater.inflate(R.menu.game, menu)
        contextMenuGame = game
    }

    override fun onContextItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.menuItemGameRun -> {
                contextMenuGame?.let { g -> runGame(g) }
                true
            }

            R.id.menuItemGameInfo -> {
                requireActivity().runOnUiThread {
                    UIHelper.showError(requireActivity(), "Not implemented yet")
                }
                true
            }

            R.id.menuItemGameSettings -> {
                requireActivity().runOnUiThread {
                    UIHelper.showError(requireActivity(), "Not implemented yet")
                }
                true
            }

            R.id.menuItemGameShareLogs -> {
                gameShareLogs(contextMenuGame!!)
                true
            }

            else -> super.onContextItemSelected(item)
        }
    }

    fun runGame(game: Game) {
        val a = requireActivity()
        game.customProperties[Game.PROPERTY_OWNER_PACKAGE]?.let {
            p -> runGamePackage(game, p)
            return
        }

        UIHelper.showError(a, getString(R.string.message_run_game_unknown_source))
    }

    private data class AppVersion(
        val versionName: String,
        val versionNumber: Long,
    )

    private fun getAppVersion(context: Context): AppVersion? {
        return try {
            val packageManager = context.packageManager
            val packageName = context.packageName
            val packageInfo = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                packageManager.getPackageInfo(packageName, PackageManager.PackageInfoFlags.of(0))
            } else {
                packageManager.getPackageInfo(packageName, 0)
            }
            AppVersion(
                versionName = packageInfo.versionName,
                versionNumber = PackageInfoCompat.getLongVersionCode(packageInfo),
            )
        } catch (e: Exception) {
            null
        }
    }

    fun gameShareLogs(game: Game) {
        val a = requireActivity()
        val l = launcher!!
        val fileZip = File(a.cacheDir, "AndroidLauncherLogs.zip")

        // collect information
        val gameInfo: MutableList<String> = mutableListOf()
        gameInfo.add("identifier: ${game.identifier}")
        gameInfo.add("aliasIdentifier: ${game.aliasIdentifier}")
        gameInfo.add("title: ${game.title}")
        gameInfo.add("dragengine-version: ${l.engineVersion}")

        val appVersion = getAppVersion(a)
        gameInfo.add("launcher-versionCode: ${appVersion?.versionNumber}")
        gameInfo.add("launcher-versionName: ${appVersion?.versionName}")

        val bodyText: MutableList<String> = mutableListOf()
        bodyText.add("Please provide the following information if known:")
        bodyText.add("- What problem did you observe?")
        bodyText.add("- What outcome did you expect instead?")
        bodyText.add("- What did you do right before the problem occured?")
        bodyText.add("- Other information than might help to investigate the problem")
        bodyText.add("")

        // create zip archive
        try {
            FileOutputStream(fileZip).use { fos ->
                val dirLogs = File(l.pathLauncherConfig, "logs")
                val dirGames = File(dirLogs, "games")
                val dirGame = File(dirGames, game.identifier)
                val dirGameLogs = File(dirGame, "logs")

                ZipOutputStream(fos).use { zos ->
                    // launcher logs
                    var file = File(dirLogs, "${l.logFilename}.log")
                    if (file.exists() && file.canRead()) {
                        FileInputStream(file).use { fis ->
                            zos.putNextEntry(ZipEntry("launcher.log"))
                            fis.copyTo(zos)
                            zos.closeEntry()
                        }
                    }

                    file = File(dirLogs, "launcher-engine.log")
                    if (file.exists() && file.canRead()) {
                        FileInputStream(file).use { fis ->
                            zos.putNextEntry(ZipEntry("launcher-engine.log"))
                            fis.copyTo(zos)
                            zos.closeEntry()
                        }
                    }

                    // game logs
                    file = File(dirGameLogs, "last_run.log")
                    if (file.exists() && file.canRead()) {
                        FileInputStream(file).use { fis ->
                            zos.putNextEntry(ZipEntry("game-last_run.log"))
                            fis.copyTo(zos)
                            zos.closeEntry()
                        }
                    }

                    // game information
                    zos.putNextEntry(ZipEntry("information"))
                    zos.write((gameInfo + "").joinToString("\n").encodeToByteArray())
                    zos.closeEntry()
                }
            }
        } catch (e: Exception) {
            e.message?.let { m -> UIHelper.showError(a, m) }
                ?: UIHelper.showError(a, getString(R.string.message_fail_create_log_zip))
            return
        }

        // share
        try {
            val uri = FileProvider.getUriForFile(a, "${a.packageName}.provider", fileZip)
            val mimeType = "application/zip"

            val shareIntent: Intent = Intent().apply {
                action = Intent.ACTION_SEND
                clipData = ClipData("DELauncher Log Files", arrayOf(mimeType), ClipData.Item(uri))
                putExtra(Intent.EXTRA_STREAM, uri)
                addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
                type = mimeType
                putExtra(Intent.EXTRA_EMAIL, arrayOf("info@dragondreams.ch"))
                putExtra(Intent.EXTRA_SUBJECT, "Android Launcher: Support")
                putExtra(Intent.EXTRA_TEXT, (bodyText + "").joinToString("\n"))
            }
            startActivity(Intent.createChooser(shareIntent, null))
        } catch (e: Exception) {
            e.message?.let { m -> UIHelper.showError(a, m) }
                ?: UIHelper.showError(a, getString(R.string.message_failed_sharing_logs))
            return
        }
    }

    private fun runGamePackage(game: Game, ownerPackage: String) {
        val a = requireActivity()
        if (ownerPackage == a.packageName) {
            examples.find { e -> e.gameId == game.identifier }?.run(a)
            return
        }

        var intent = a.packageManager.getLaunchIntentForPackage(ownerPackage)
        Log.i(TAG, "intent: $intent")

        if (intent == null) {
            intent = Intent(Intent.ACTION_MAIN)
            intent.setClassName(ownerPackage, "${ownerPackage}.MainActivity")
        }

        intent.putExtra(RunDelgaActivity.EXTRA_GAME_ID, game.identifier)
        try {
            a.startActivity(intent)
        } catch (e: Exception) {
            e.message?.let { m -> UIHelper.showError(a, m) }
                ?: UIHelper.showError(a, R.string.error_example_run)
        }
    }

    companion object {
        const val TAG = "FragmentGames"
    }
}