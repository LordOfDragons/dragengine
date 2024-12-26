package ch.dragondreams.delauncher.launcher.internal

import android.view.SurfaceView
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule
import ch.dragondreams.delauncher.launcher.EngineModuleParameter
import ch.dragondreams.delauncher.launcher.EngineModuleParameterInfo

class Launcher(owner: DragengineLauncher, view: SurfaceView?)  {
    private var nativeLauncher: Long = 0L

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

    private external fun createLauncher(config: LauncherConfig): Long
    private external fun destroyLauncher(launcher: Long)
    private external fun getEngineModules(launcher: Long):
            Array<ch.dragondreams.delauncher.launcher.internal.EngineModule>

    init {
        nativeLauncher = createLauncher(
            LauncherConfig(
                "Launcher",
                "launcher-engine",
                owner.pathLauncher.absolutePath,
                owner.pathLauncherGames.absolutePath,
                owner.pathLauncherConfig.absolutePath,
                owner.pathEngine.absolutePath,
                owner.pathEngineConfig.absolutePath,
                owner.pathEngineCache.absolutePath,
                null)
        )
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