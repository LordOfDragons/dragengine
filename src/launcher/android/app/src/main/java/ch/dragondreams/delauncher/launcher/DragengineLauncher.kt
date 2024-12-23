package ch.dragondreams.delauncher.launcher

import android.content.Context
import android.util.Log
import java.io.File
import java.io.FileOutputStream
import java.io.InputStream
import java.util.zip.ZipInputStream
import kotlin.time.TimeSource

class DragengineLauncher(context: Context) {
    private val TAG: String = "DragengineLauncher"

    val pathEngine: File

    init {
        pathEngine = File(context.filesDir, "dragengine")

        // delete unpacked files
        deleteDirectory(pathEngine)

        // unpack archive
        context.assets.open("dragengine.zip").use { s ->
            unzipAsset(s, pathEngine)
        }

        // test load library
        System.loadLibrary("dragengine")
        System.loadLibrary("delauncher")
        System.loadLibrary("launcherglue")
    }

    private fun deleteDirectory(directory: File) {
        if (directory.exists() && directory.isDirectory) {
            directory.listFiles()?.forEach { file ->
                if (file.isDirectory) {
                    deleteDirectory(file)
                } else {
                    file.delete()
                }
            }
            directory.delete()
        }
    }

    private fun unzipAsset(stream: InputStream, targetPath: File) {
        if (!targetPath.exists()) {
            targetPath.mkdir()
        }

        ZipInputStream(stream).use { zis ->
            val buffer = ByteArray(1024 * 8)
            var length = 0

            var entry = zis.nextEntry
            while (entry != null) {
                if (!entry.isDirectory) {
                    val fileEntry = File(targetPath, entry.name)
                    // Log.i(TAG, "unzipAsset: Unpack " + entry.name)

                    val fileEntryDir = fileEntry.parentFile
                    if (!fileEntryDir.exists()) {
                        if (!fileEntryDir.mkdirs()) {
                            throw Exception("Failed creating directory " + fileEntryDir.name)
                        }
                    }

                    FileOutputStream(fileEntry).use { fos ->
                        while ((zis.read(buffer).also { length = it }) > 0) {
                            fos.write(buffer, 0, length)
                            fos.flush()
                        }
                    }
                }
                entry = zis.nextEntry
            }
        }
    }
}
