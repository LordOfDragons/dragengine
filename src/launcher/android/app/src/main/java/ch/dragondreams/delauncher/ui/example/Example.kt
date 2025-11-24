package ch.dragondreams.delauncher.ui.example

import android.app.Activity
import android.content.ActivityNotFoundException
import android.content.Intent
import androidx.core.content.FileProvider
import ch.dragondreams.delauncher.FileDownloader
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.RunDelgaActivity
import ch.dragondreams.delauncher.UIHelper
import java.io.File
import java.io.IOException
import kotlin.concurrent.Volatile

class Example(
    val name: String,
    val url: String,
    val gameId: String){

    enum class State{
        NotReady,
        Downloading,
        Ready
    }

    @Volatile
    var state = State.NotReady

    val delgaFilename: String = url.split('/').last()

    var pathInstalled: File? = null
    var pathDownloading: File? = null

    @Volatile
    var downloadProgress = 0

    @Volatile
    var uiUpdatePending = false

    var downloader: FileDownloader? = null

    fun updateState() {
        state = if (pathInstalled?.exists() == true) {
            State.Ready
        } else {
            State.NotReady
        }
    }

    fun moveDownloadToInstalled() {
        val pi = pathInstalled!!
        val pd = pathDownloading!!

        try {
            if (pi.exists() && !pi.delete()) {
                throw IOException("Can not delete installed file")
            }
            pd.renameTo(pi)

        } catch (e: Exception) {
            if (pi.exists()) {
                try {
                    pi.delete()
                } catch (_: Exception) {}
            }
            if (pd.exists()) {
                try {
                    pd.delete()
                } catch (_: Exception) {}
            }
        }
    }

    fun deleteInstalledFile() {
        val pi = pathInstalled!!
        if (pi.exists() && !pi.delete()) {
            throw IOException("Can not delete installed file")
        }
    }

    fun run(activity: Activity): Boolean {
        if (state != State.Ready) {
            activity.runOnUiThread {
                UIHelper.showError(activity, R.string.error_example_not_ready)
            }
            return false
        }

        val intent = Intent().apply {
            action = RunDelgaActivity.ACTION_LAUNCH_DELGA
            setDataAndType(
                FileProvider.getUriForFile(activity,
                    "${activity.packageName}.provider", pathInstalled!!),
                RunDelgaActivity.MIME_TYPE_DELGA)
            addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
            //addFlags(Intent.FLAG_DEBUG_LOG_RESOLUTION)
            addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
            addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK)
            putExtra(RunDelgaActivity.EXTRA_OWNER_PACKAGE, activity.packageName)
            putExtra(RunDelgaActivity.EXTRA_GAME_ID, gameId)
        }

        try {
            activity.startActivity(intent)
        } catch (e: ActivityNotFoundException) {
            activity.runOnUiThread {
                e.message?.let { m -> UIHelper.showError(activity, m) }
                    ?: UIHelper.showError(activity, R.string.error_example_run)
            }
            return false
        }
        return true
    }
}
