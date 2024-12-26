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

    private class GlueLauncher(owner: DragengineLauncher, view: SurfaceView?) {
        private var nativeLauncher: Long = 0L

        private data class GlueLauncherConfig(
            var loggerSource: String = "",
            var engineLogFileTitle: String = "",
            var pathLauncher: String = "",
            var pathLauncherGames: String = "",
            var pathLauncherConfig: String = "",
            var pathEngine: String = "",
            var pathEngineConfig: String = "",
            var pathEngineCache: String = "",
            var surface: SurfaceView? = null
        )

        private val mapModuleParameterInfoType: Map<Int, EngineModuleParameterInfo.Type> = mapOf(
            0 to EngineModuleParameterInfo.Type.Boolean,
            1 to EngineModuleParameterInfo.Type.Numeric,
            2 to EngineModuleParameterInfo.Type.Ranged,
            3 to EngineModuleParameterInfo.Type.String,
            4 to EngineModuleParameterInfo.Type.Selection)

        private val mapModuleParameterInfoCategory: Map<Int, EngineModuleParameterInfo.Category> = mapOf(
            0 to EngineModuleParameterInfo.Category.Basic,
            1 to EngineModuleParameterInfo.Category.Advanced,
            2 to EngineModuleParameterInfo.Category.Expert)

        private data class GlueEngineModuleParameterInfoSelectionEntry(
            var value: String = "",
            var displayName: String = "",
            var description: String = "")

        private data class GlueEngineModuleParameterInfo(
            var type: Int = 0, // EngineModuleParameter.Type
            var name: String = "",
            var description: String = "",
            var minValue: Float = 0.0F,
            var maxValue: Float = 0.0F,
            var valueStepSize: Float = 0.0F,
            var selectionEntries: Array<GlueEngineModuleParameterInfoSelectionEntry> = emptyArray(),
            var category: Int = 0, // EngineModuleParameter.Category
            var displayName: String = "",
            var defaultValue: String = "")

        private data class GlueEngineModuleParameter(
            var index: Int = 0,
            var info: GlueEngineModuleParameterInfo = GlueEngineModuleParameterInfo(),
            var value: String = "")

        private val mapModuleType: Map<Int, EngineModule.Type> = mapOf(
            0 to EngineModule.Type.Unknown,
            1 to EngineModule.Type.Graphic,
            2 to EngineModule.Type.Audio,
            3 to EngineModule.Type.Input,
            4 to EngineModule.Type.Network,
            5 to EngineModule.Type.Physics,
            6 to EngineModule.Type.Image,
            7 to EngineModule.Type.Video,
            8 to EngineModule.Type.Script,
            9 to EngineModule.Type.Model,
            10 to EngineModule.Type.Rig,
            11 to EngineModule.Type.Skin,
            12 to EngineModule.Type.Animation,
            13 to EngineModule.Type.Font,
            14 to EngineModule.Type.CrashRecovery,
            15 to EngineModule.Type.LanguagePack,
            16 to EngineModule.Type.Animator,
            17 to EngineModule.Type.Sound,
            18 to EngineModule.Type.AI,
            19 to EngineModule.Type.OcclusionMesh,
            20 to EngineModule.Type.Synthesizer,
            21 to EngineModule.Type.Archive,
            22 to EngineModule.Type.VR,
            23 to EngineModule.Type.Service)

        private val mapModuleStatus: Map<Int, EngineModule.Status> = mapOf(
            0 to EngineModule.Status.Ready,
            1 to EngineModule.Status.NotTested,
            2 to EngineModule.Status.Broken)

        private data class GlueEngineModule(
            var type: Int = 0, // EngineModule.Type
            var name: String = "",
            var description: String = "",
            var author: String = "",
            var version: String = "",
            var dirName: String = "",
            var pattern: String = "",
            var priority: Int = 0,
            var isFallback: Boolean = false,
            var status: Int = 0, // EngineModule.Status
            var errorCode: Int = 0,
            var libFileName: String = "",
            var libFileSizeShould: Int = 0,
            var libFileSizeIs: Int = 0,
            var libFileHashShould: String = "",
            var libFileHashIs: String = "",
            var libFileEntryPoint: String = "",
            var parameters: Array<GlueEngineModuleParameter> = emptyArray())

        private external fun createLauncher(config: GlueLauncherConfig): Long
        private external fun destroyLauncher(launcher: Long)
        private external fun getEngineModules(launcher: Long): Array<GlueEngineModule>

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

        fun getEngineModules(): List<EngineModule>{
            val modules = ArrayList<EngineModule>();
            getEngineModules(nativeLauncher).forEach { gem ->
                val module = EngineModule()
                module.type = mapModuleType[gem.type]!!
                module.name = gem.name
                module.description = gem.description
                module.author = gem.author
                module.version = gem.version
                module.dirName = gem.dirName
                module.pattern = gem.pattern
                module.priority = gem.priority
                module.isFallback = gem.isFallback
                module.status = mapModuleStatus[gem.status]!!
                module.errorCode = gem.errorCode
                module.libFileName = gem.libFileName
                module.libFileSizeShould = gem.libFileSizeShould
                module.libFileSizeIs = gem.libFileSizeIs
                module.libFileHashShould = gem.libFileHashShould
                module.libFileHashIs = gem.libFileHashIs
                module.libFileEntryPoint = gem.libFileEntryPoint
                module.parameters = gem.parameters.map { p ->
                    val parameter = EngineModuleParameter()
                    parameter.index = p.index
                    parameter.value = p.value
                    parameter.info.type = mapModuleParameterInfoType[p.info.type]!!
                    parameter.info.name = p.info.name
                    parameter.info.description = p.info.description
                    parameter.info.minValue = p.info.minValue.toDouble()
                    parameter.info.maxValue = p.info.maxValue.toDouble()
                    parameter.info.valueStepSize = p.info.valueStepSize.toDouble()
                    parameter.info.selectionEntries = p.info.selectionEntries.map { e ->
                        val entry = EngineModuleParameterInfo.SelectionEntry()
                        entry.value = e.value
                        entry.displayName = e.displayName
                        entry.description = e.description
                        entry
                    }.toTypedArray()
                    parameter.info.category = mapModuleParameterInfoCategory[p.info.category]!!
                    parameter.info.displayName = p.info.displayName
                    parameter.info.defaultValue = p.info.defaultValue
                    parameter
                }.toTypedArray()
                modules.add(module)
            }
            return modules
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

        var launcher: GlueLauncher? = null

        try {
            launcher = GlueLauncher(this, null)

            val modules = launcher.getEngineModules()
            Log.i(tag, "createGlueLauncher: Engine modules: ${modules.size}")
            modules.forEach { m ->
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
            setState(State.CreateGlueLauncherFailed)
            launcher?.dispose()
            return
        }

        this.launcher = launcher
        Log.i(tag, "game engine ready")
        setState(State.EngineReady)
    }
}
