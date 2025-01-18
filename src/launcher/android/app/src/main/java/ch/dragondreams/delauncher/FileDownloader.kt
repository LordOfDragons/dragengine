package ch.dragondreams.delauncher

import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import java.io.BufferedInputStream
import java.io.File
import java.io.FileOutputStream
import java.net.HttpURLConnection
import java.net.URL
import kotlin.concurrent.Volatile

class FileDownloader(
    val url: String,
    val file: File,
    private val listener: DownloadListener
) {
    interface DownloadListener {
        fun onProgress(downloader: FileDownloader, progress: Int)
        fun onSuccess(downloader: FileDownloader)
        fun onFailed(downloader: FileDownloader, exception: Exception)
        fun onCancelled(downloader: FileDownloader)
    }

    @Volatile
    var cancelDownload = false

    fun download() {
        MainScope().launch(Dispatchers.IO) {
            try {
                doDownload()
            } catch (e: Exception) {
                deleteDownloadFile()
                listener.onFailed(this@FileDownloader, e)
            }
        }
    }

    private fun doDownload() {
        val connection = URL(url).openConnection() as HttpURLConnection
        val contentLength = connection.contentLength

        if (file.parentFile?.exists() == false) {
            file.parentFile?.mkdirs()
        }

        BufferedInputStream(connection.inputStream).use { bis ->
            FileOutputStream(file.path).use { fos ->
                val buffer = ByteArray(4096)
                var downloadedFileSize = 0L
                var currentRead = 0

                while (currentRead != -1 && !cancelDownload) {
                    downloadedFileSize += currentRead
                    fos.write(buffer, 0, currentRead)
                    currentRead = bis.read(buffer, 0, buffer.size)
                    val progress = (100f * (downloadedFileSize.toFloat() / contentLength.toFloat())).toInt()
                    listener.onProgress(this, progress)
                }
            }
        }

        if(cancelDownload) {
            deleteDownloadFile()
            listener.onCancelled(this)

        }else{
            listener.onSuccess(this)
        }
    }

    fun deleteDownloadFile() {
        if (file.exists()) {
            try {
                file.delete()
            } catch (e: Exception) {
                Log.i("FileDownloader", "download: ${e.message}")
            }
        }
    }
}
