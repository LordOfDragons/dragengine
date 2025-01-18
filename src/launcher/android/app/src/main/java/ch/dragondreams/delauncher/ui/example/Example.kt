package ch.dragondreams.delauncher.ui.example

import ch.dragondreams.delauncher.FileDownloader
import java.io.File
import java.io.IOException
import kotlin.concurrent.Volatile

class Example(
    val name: String,
    val url: String){

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
            Example.State.Ready
        } else {
            Example.State.NotReady
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
}
