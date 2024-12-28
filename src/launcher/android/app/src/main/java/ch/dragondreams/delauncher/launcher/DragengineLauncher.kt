package ch.dragondreams.delauncher.launcher

import android.content.Context
import android.util.Log
import ch.dragondreams.delauncher.launcher.internal.Launcher
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

        /** Engine modules changed. */
        fun engineModulesChanged(launcher: DragengineLauncher)
    }

    open class DefaultListener : Listener {
        override fun stateChanged(launcher: DragengineLauncher) { }
        override fun engineModulesChanged(launcher: DragengineLauncher) { }
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
         * Create internal launcher instance.
         */
        CreateInternalLauncher,

        /**
         * Create internal launcher instance failed.
         */
        CreateInternalLauncherFailed,

        /**
         * Game engine is installed and ready to be used.
         */
        EngineReady
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
    val engineModules: MutableList<EngineModule> = ArrayList()

    private var launcher: Launcher? = null

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

    private var listeners: MutableList<Listener> = ArrayList()
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
            System.loadLibrary("launcherinternal")

        } catch (e: Exception) {
            Log.e(tag, "loadLibraries: Failed", e)
            setState(State.LoadLibrariesFailed)
            return
        }

        createInternalLauncher()
    }

    private fun createInternalLauncher() {
        Log.i(tag, "createGlueLauncher")
        setState(State.CreateInternalLauncher)

        var launcher: Launcher? = null
        try {
            launcher = Launcher(this, null)

            engineModules.clear()
            engineModules.addAll(launcher.getEngineModules())

            try {
                listenersLocked++
                Log.i(tag, "notifyEngineModulesChanged: ${listeners.size} ${listenersLocked}")
                listeners.forEach { each -> each.engineModulesChanged(this) }
            } finally {
                listenersLocked--
            }

            Log.i(tag, "createGlueLauncher: Engine modules: ${engineModules.size}")
            engineModules.forEach { m ->
                Log.i(tag, "createGlueLauncher: - ${m.name} (${m.type.name}) ${m.isFallback} '${m.author}'")
                m.parameters.forEach { p ->
                    Log.i(tag, "createGlueLauncher: -> ${p.index} '${p.value}': '${p.info.name}' (${p.info.type}) [${p.info.minValue} ${p.info.maxValue}]")
                    p.info.selectionEntries.forEach { e ->
                        Log.i(tag, "createGlueLauncher:    {=} '${e.displayName}' -> '${e.value}'")
                    }
                }
            }

        } catch (e: Exception) {
            Log.e(tag, "createGlueLauncher: Failed", e)
            setState(State.CreateInternalLauncherFailed)
            launcher?.dispose()
            return
        }

        this.launcher = launcher
        Log.i(tag, "game engine ready")
        setState(State.EngineReady)
    }
}
