package ch.dragondreams.delauncher.launcher.internal

import android.view.SurfaceView
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule

class Launcher(owner: DragengineLauncher, view: SurfaceView?)  {
    private var nativeLauncher: Long = 0L

    private external fun createLauncher(config: LauncherConfig): Long
    private external fun destroyLauncher(launcher: Long)
    private external fun getEngineModules(launcher: Long):
            Array<ch.dragondreams.delauncher.launcher.internal.EngineModule>
    private external fun getGames(launcher: Long): Array<Game>

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

    fun getEngineModules(): Collection<EngineModule>{
        val modules = mutableListOf<EngineModule>();
        getEngineModules(nativeLauncher).forEach { m -> modules.add(m.convert()) }
        return modules
    }

    fun getGames(): Collection<ch.dragondreams.delauncher.launcher.Game> {
        val games = mutableListOf<ch.dragondreams.delauncher.launcher.Game>()
        getGames(nativeLauncher).forEach { g -> games.add(g.convert()) }
        return games
    }
}
