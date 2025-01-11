package ch.dragondreams.delauncher.launcher

import android.content.Context
import android.view.SurfaceView
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

class DragengineLauncher(
    val context: Context,
    val view: SurfaceView?
) {
    interface Listener {
        /** State changed. */
        fun stateChanged(launcher: DragengineLauncher)

        /** Engine modules changed. */
        fun engineModulesChanged(launcher: DragengineLauncher)

        /** Games changed. */
        fun gamesChanged(launcher: DragengineLauncher)

        /** Game profiles changed. */
        fun gameProfilesChanged(launcher: DragengineLauncher)
    }

    open class DefaultListener : Listener {
        override fun stateChanged(launcher: DragengineLauncher) { }
        override fun engineModulesChanged(launcher: DragengineLauncher) { }
        override fun gamesChanged(launcher: DragengineLauncher) { }
        override fun gameProfilesChanged(launcher: DragengineLauncher) { }
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
         * Installing/updating game engine failed. GameInfo engine is not ready to be used.
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
         * GameInfo engine is installed and ready to be used.
         */
        EngineReady
    }

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
    val engineModules: MutableList<EngineModule> = mutableListOf()
    val games: MutableList<Game> = mutableListOf()
    val gameProfiles: MutableList<GameProfile> = mutableListOf()

    /** Default game profile. */
    var defaultProfile: GameProfile? = null
        private set

    /** Active profile. */
    var activeProfile: GameProfile? = null
        set(value) {
            launcher!!.setActiveProfile(value)
            field = value
        }


    var launcher: Launcher? = null
        private set

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

    /**
     * Filename relative to data directory to log to or null to not log to file.
     * Default is "delauncher".
     */
    var logFilename: String = "delauncher"

    private var listeners: MutableList<Listener> = ArrayList()
    private var listenersLocked: Int = 0

    private val crscope = CoroutineScope(Dispatchers.IO)

    fun dispose() {
        activeProfile?.release()
        activeProfile = null

        defaultProfile?.release()
        defaultProfile = null

        gameProfiles.forEach { p -> p.release() }
        gameProfiles.clear()

        games.forEach { g -> g.release() }
        games.clear()

        launcher?.dispose()
        launcher = null
    }


    /**
     * Start initializing and creating launcher. Call after adding listeners and
     * setting parameters required to be done before this process begins.
     */
    fun initLauncher() {
        crscope.launch {
            verifyEngineInstallation(context)
        }
    }

    /**
     * Add file logger. Path has to be an absolute file path.
     */
    fun addFileLogger(path: String){
        launcher?.addFileLogger(path)
    }

    /**
     * Add native file descriptor as path to VFS. File descriptor will be closed
     * upon removal from VFS. Path is added to VFS under "/fds".
     */
    fun vfsContainerAddFd(path: String, producer: Launcher.FileDescriptorProducer,
                          offset: Int, length: Int) {
        launcher?.vfsContainerAddFd(path, producer, offset, length)
    }

    /**
     * Remove native file descriptor from VFS. File descriptor will be closed.
     */
    fun vfsContainerRemoveFd(path: String) {
        launcher?.vfsContainerRemoveFd(path)
    }

    /**
     * Read game definitions from DELGA file.
     */
    fun readDelgaGames(path: String): List<Game> {
        return launcher?.readDelgaGames(path) ?: emptyList()
    }

    /**
     * Game engine module type is single type.
     */
    fun isModuleSingleType(type: EngineModule.Type): Boolean {
        return launcher?.isModuleSingleType(type) == true
    }

    /**
     * Create new game instance.
     */
    fun createGame(): Game{
        return Game.getInstance(launcher?.createGame())!!
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
        logInfo("verifyEngineInstallation")
        setState(State.VerifyEngineInstallation)

        val versionInstalled = getEngineVersionInstalled(context)
        engineVersion = getEngineVersionInstall(context)
        logInfo("verifyEngineInstallation: version '$engineVersion' installed '$versionInstalled'")

        if (engineVersion == versionInstalled) {
            loadLibraries()

        } else {
            installEngine(context)
            updateEngineVersionInstalled(context, engineVersion)
        }
    }

    private fun installEngine(context: Context) {
        logInfo("installEngine")
        setState(State.InstallEngine)

        try {
            logInfo("installEngine: delete old engine")
            deleteDirectory(pathEngine)

            logInfo("installEngine: unpack engine")
            unpackEngine(context, pathEngine)

        } catch (e: Exception) {
            logError("installEngine: Failed", e)
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
        logInfo("unzipAsset: Entries extracted in "
                + timer.elapsedNow().inWholeMilliseconds + "ms")
    }

    private fun loadLibraries() {
        logInfo("loadLibraries")
        setState(State.LoadLibraries)

        try {
            System.loadLibrary("dragengine")
            System.loadLibrary("delauncher")
            System.loadLibrary("launcherinternal")

        } catch (e: Exception) {
            logError("loadLibraries: Failed", e)
            setState(State.LoadLibrariesFailed)
            return
        }

        createInternalLauncher()
    }

    private fun createInternalLauncher() {
        logInfo("createInternalLauncher")
        setState(State.CreateInternalLauncher)

        try {
            launcher = Launcher(this, view)
            launcher?.addFileLogger(logFilename)
            launcher?.prepare()

            engineModules.clear()
            engineModules.addAll(launcher!!.getEngineModules())

            games.forEach { g -> g.release() }
            games.clear()
            games.addAll(launcher!!.getGames())

            gameProfiles.forEach { p -> p.release() }
            gameProfiles.clear()
            gameProfiles.addAll(launcher!!.getGameProfiles())

            defaultProfile?.release()
            defaultProfile = launcher!!.getDefaultProfile()

            activeProfile?.release()
            activeProfile = launcher!!.getDefaultProfile()

            try {
                listenersLocked++
                logInfo("notifyEngineModulesChanged: ${listeners.size} ${listenersLocked}")
                listeners.forEach { each -> each.engineModulesChanged(this) }
            } finally {
                listenersLocked--
            }

            /*
            logInfo("createInternalLauncher: Engine modules: ${engineModules.size}")
            engineModules.forEach { m ->
                logInfo("createInternalLauncher: - ${m.name} (${m.type.name}) ${m.isFallback} '${m.author}'")
                m.parameters.forEach { p ->
                    logInfo("createInternalLauncher: -> ${p.index} '${p.value}': '${p.info.name}' (${p.info.type}) [${p.info.minValue} ${p.info.maxValue}]")
                    p.info.selectionEntries.forEach { e ->
                        logInfo("createInternalLauncher:    {=} '${e.displayName}' -> '${e.value}'")
                    }
                }
            }
            */

        } catch (e: Exception) {
            logError("createInternalLauncher: Failed", e)
            setState(State.CreateInternalLauncherFailed)
            launcher?.dispose()
            launcher = null
            return
        }

        logInfo("game engine ready")
        setState(State.EngineReady)
    }

    fun logInfo(message: String){
        launcher?.logInfo(message)
    }

    fun logWarn(message: String){
        launcher?.logWarn(message)
    }

    fun logError(message: String){
        launcher?.logError(message)
    }

    fun logError(message: String, exception: Exception){
        launcher?.logError(message + ":\n" + exception.message + "\n" + exception.stackTraceToString())
    }

    companion object {
        private const val TAG = "DragengineLauncher"
    }
}
