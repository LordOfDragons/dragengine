package ch.dragondreams.delauncher.launcher

import android.content.Context
import android.util.Log
import android.view.SurfaceView
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.util.zip.ZipInputStream
import kotlin.concurrent.Volatile
import kotlin.math.min
import kotlin.time.TimeSource

class DragengineLauncher(context: Context) {
    interface Listener {
        /** State changed. */
        fun stateChanged(launcher: DragengineLauncher)
    }

    open class DefaultListener : Listener {
        override fun stateChanged(launcher: DragengineLauncher) {
        }
    }

    enum class State {
        /**
         * Verify if game engine is properly installed.
         * State after creating instance of DragengineLauncher.
         */
        VerifyEngineInstallation,

        /**
         * Installing/updating game engine. Set after verifyEngineInstallation
         * if the game engine is not installed or outdated.
         */
        InstallEngine,

        /**
         * Installing/updating game engine failed. Game engine is not ready to be used.
         */
        InstallEngineFailed,

        /**
         * Loading game engine libraries.
         */
        LoadLibraries,

        /**
         * Loading game engine libraries failed.
         */
        LoadLibrariesFailed,

        /**
         * Create glue launcher instance.
         */
        CreateGlueLauncher,

        /**
         * Create glue launcher instance failed.
         */
        CreateGlueLauncherFailed,

        /**
         * Game engine is installed and ready to be used.
         */
        EngineReady
    }

    class GlueLauncher(owner: DragengineLauncher, view: SurfaceView?) {
        private var nativeLauncher: Long = 0L

        private data class GlueLauncherConfig(
            val loggerSource: String,
            val engineLogFileTitle: String,
            val pathLauncher: String,
            val pathLauncherGames: String,
            val pathLauncherConfig: String,
            val pathEngine: String,
            val pathEngineConfig: String,
            val pathEngineCache: String,
            val surface: SurfaceView?
        )

        private external fun createLauncher(config: GlueLauncherConfig): Long
        private external fun destroyLauncher(launcher: Long)

        init {
            nativeLauncher = createLauncher(
                GlueLauncherConfig(
                    "Launcher",
                    "launcher-engine",
                    owner.pathLauncher.absolutePath,
                    owner.pathLauncherGames.absolutePath,
                    owner.pathLauncherConfig.absolutePath,
                    owner.pathEngine.absolutePath,
                    owner.pathEngineConfig.absolutePath,
                    owner.pathEngineCache.absolutePath,
                    null))
        }

        fun dispose() {
            if (nativeLauncher != 0L){
                destroyLauncher(nativeLauncher)
            }
        }
    }

    private val tag = "DragengineLauncher"

    val pathEngine = File(context.filesDir, "dragengine")
    //val pathEngineConfig = File(context.filesDir, "dragengine-config")
    val pathEngineConfig = File(context.getExternalFilesDir(null)!!, "dragengine-config")
    val pathEngineCache = File(context.cacheDir, "dragengine")
    val pathLauncher = File(context.filesDir, "delauncher")
    //val pathLauncherConfig = File(context.filesDir, "delauncher-config")
    val pathLauncherConfig = File(context.getExternalFilesDir(null)!!, "delauncher-config")
    val pathLauncherGames = File(context.filesDir, "delauncher-games")

    var engineVersion = ""
        private set

    private var launcher: GlueLauncher? = null

    /**
     * State of launcher.
     */
    @Volatile
    var state: State = State.VerifyEngineInstallation
        private set

    /**
     * Progress of installing/updating game engine as percentage value
     * in the range from 0 to 1.
     */
    @Volatile
    var progressInstallEngine: Double = 0.0

    private var listeners: List<Listener> = ArrayList()
    private var listenersLocked: Int = 0

    private val crscope = CoroutineScope(Dispatchers.IO)

    init {
        crscope.launch {
            verifyEngineInstallation(context)
        }
    }

    fun dispose() {
        launcher?.dispose()
        launcher = null
    }


    /** Add listener. */
    @Synchronized
    fun addListener(listener: Listener) {
        if (listenersLocked > 0) {
            listeners = ArrayList(listeners)
        }
        listeners += listener
    }

    /** Remove listener. */
    @Synchronized
    fun removeListener(listener: Listener) {
        if (listenersLocked > 0) {
            listeners = ArrayList(listeners)
        }
        listeners -= listener
    }

    private fun setState(state: State) {
        if (state == this.state) {
            return
        }

        this.state = state

        try {
            listenersLocked++
            listeners.forEach { each -> each.stateChanged(this) }
        } finally {
            listenersLocked--
        }
    }

    private fun verifyEngineInstallation(context: Context) {
        Log.i(tag, "verifyEngineInstallation")
        setState(State.VerifyEngineInstallation)

        val versionInstalled = getEngineVersionInstalled(context)
        val versionInstall = getEngineVersionInstall(context)
        Log.i(tag, "verifyEngineInstallation: version '$versionInstall' installed '$versionInstalled'")

        if (versionInstall == versionInstalled) {
            loadLibraries()

        } else {
            installEngine(context)
            updateEngineVersionInstalled(context, versionInstall)
        }
    }

    private fun installEngine(context: Context) {
        Log.i(tag, "installEngine")
        setState(State.InstallEngine)

        try {
            Log.i(tag, "installEngine: delete old engine")
            deleteDirectory(pathEngine)

            Log.i(tag, "installEngine: unpack engine")
            unpackEngine(context, pathEngine)

        } catch (e: Exception) {
            Log.e(tag, "installEngine: Failed", e)
            setState(State.InstallEngineFailed)
            return
        }

        loadLibraries()
    }

    private fun getEngineVersionInstall(context: Context): String {
        var v = context.assets.open("dragengine-version").use {
            s -> s.readBytes().decodeToString()
        }
        if (v[v.length - 1] == '\n') {
            v = v.substring(0, v.length - 1)
        }
        return v
    }

    private fun getEngineVersionInstalled(context: Context): String {
        val file = File(context.filesDir, "dragengine-version")
        if (!file.exists()) {
            return ""
        }

        var v = FileInputStream(file).use { s ->
            s.readBytes().decodeToString()
        }
        if (v[v.length - 1] == '\n') {
            v = v.substring(0, v.length - 1)
        }
        return v
    }

    private fun updateEngineVersionInstalled(context: Context, version: String) {
        val file = File(context.filesDir, "dragengine-version")
        FileOutputStream(file).use { fos ->
            fos.write(version.encodeToByteArray())
            fos.flush()
        }

        this.engineVersion = version
    }

    private fun getEngineArchiveSize(context: Context): Long {
        var t = context.assets.open("dragengine-filesize").use {
                s -> s.readBytes().decodeToString()
        }
        if (t[t.length - 1] == '\n') {
            t = t.substring(0, t.length - 1)
        }
        return t.toLong()
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

    private fun unpackEngine(context: Context, targetPath: File) {
        if (!targetPath.exists()) {
            targetPath.mkdir()
        }

        val archiveSize = getEngineArchiveSize(context)

        // extract files
        val timer = TimeSource.Monotonic.markNow()
        val progressFactor = if (archiveSize > 0) 1.0 / archiveSize else 0.0
        var bytesProcessed = 0L

        context.assets.open("dragengine.zip").use { s ->
            ZipInputStream(s).use { zis ->
                val buffer = ByteArray(1024 * 8)
                var length: Int

                var entry = zis.nextEntry
                while (entry != null) {
                    if (!entry.isDirectory) {
                        val fileEntry = File(targetPath, entry.name)

                        val fileEntryDir = fileEntry.parentFile
                        if (fileEntryDir?.exists() != true && fileEntryDir?.mkdirs() != true) {
                            throw Exception("Failed creating directory " + fileEntryDir?.name)
                        }

                        // var bytesEntryProcessed = 0L
                        FileOutputStream(fileEntry).use { fos ->
                            while ((zis.read(buffer).also { length = it }) > 0) {
                                fos.write(buffer, 0, length)
                                fos.flush()

                                // bytesEntryProcessed += length

                                bytesProcessed += length
                                progressInstallEngine = min(progressFactor * bytesProcessed, 1.0)
                            }
                        }

                        // bytesProcessed += bytesEntryProcessed
                        // progressInstallEngine = min(progressFactor * bytesProcessed, 1.0)
                    }
                    entry = zis.nextEntry
                }
            }
        }
        Log.i(tag, "unzipAsset: Entries extracted in "
                + timer.elapsedNow().inWholeMilliseconds + "ms")
    }

    private fun loadLibraries() {
        Log.i(tag, "loadLibraries")
        setState(State.LoadLibraries)

        try {
            System.loadLibrary("dragengine")
            System.loadLibrary("delauncher")
            System.loadLibrary("launcherglue")

        } catch (e: Exception) {
            Log.e(tag, "loadLibraries: Failed", e)
            setState(State.LoadLibrariesFailed)
            return
        }

        createGlueLauncher()
    }

    private fun createGlueLauncher() {
        Log.i(tag, "createGlueLauncher")
        setState(State.CreateGlueLauncher)

        try {
            launcher = GlueLauncher(this, null)

        } catch (e: Exception) {
            Log.e(tag, "createGlueLauncher: Failed", e)
            setState(State.CreateGlueLauncherFailed)
            return
        }

        Log.i(tag, "game engine ready")
        setState(State.EngineReady)
    }
}
