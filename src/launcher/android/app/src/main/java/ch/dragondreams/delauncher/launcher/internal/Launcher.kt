package ch.dragondreams.delauncher.launcher.internal

import android.app.Activity
import android.view.SurfaceView
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.EngineModule
import ch.dragondreams.delauncher.launcher.GameProfile

class Launcher(
    owner: DragengineLauncher,
    activity: Activity,
    view: SurfaceView?
    )
{
    interface FileDescriptorProducer {
        fun produceFileDescriptor(): Int
    }

    var nativeLauncher: Long = 0L
        private set

    private external fun createLauncher(config: LauncherConfig): Long
    private external fun destroyLauncher(launcher: Long)
    private external fun getEngineModules(launcher: Long):
            Array<ch.dragondreams.delauncher.launcher.internal.EngineModule>
    private external fun getGames(launcher: Long): LongArray
    private external fun getGameProfiles(launcher: Long): LongArray
    private external fun getDefaultProfile(launcher: Long): Long
    private external fun getActiveProfile(launcher: Long): Long
    private external fun setActiveProfile(launcher: Long, profile: Long)
    private external fun vfsContainerAddFd(launcher: Long, path: String,
           producer: FileDescriptorProducer, offset: Int, length: Int)
    private external fun vfsContainerRemoveFd(launcher: Long, path: String)
    private external fun readDelgaGames(launcher: Long, path: String): LongArray
    private external fun isModuleSingleType(type: Int): Boolean
    private external fun addFileLogger(launcher: Long, path: String)
    private external fun prepare(launcher: Long)
    private external fun logInfo(launcher: Long, message: String)
    private external fun logWarn(launcher: Long, message: String)
    private external fun logError(launcher: Long, message: String)
    private external fun createGame(launcher: Long): Long

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
                activity,
                view?.holder?.surface
            )
        )
    }

    fun dispose() {
        if (nativeLauncher != 0L){
            destroyLauncher(nativeLauncher)
        }
    }

    fun getEngineModules(): List<EngineModule>{
        val modules = mutableListOf<EngineModule>()
        getEngineModules(nativeLauncher).forEach { m -> modules.add(m.convert()) }
        return modules
    }

    fun getGames(): List<ch.dragondreams.delauncher.launcher.Game> {
        val games = mutableListOf<ch.dragondreams.delauncher.launcher.Game>()
        getGames(nativeLauncher).forEach { g ->
            games.add(ch.dragondreams.delauncher.launcher.Game.getInstance(Game.getInstance(g))!!)
        }
        return games
    }

    fun vfsContainerAddFd(path: String, producer: FileDescriptorProducer, offset: Int, length: Int) {
        vfsContainerAddFd(nativeLauncher, path, producer, offset, length)
    }

    fun vfsContainerRemoveFd(path: String) {
        vfsContainerRemoveFd(nativeLauncher, path)
    }

    fun readDelgaGames(path: String): List<ch.dragondreams.delauncher.launcher.Game> {
        val games = mutableListOf<ch.dragondreams.delauncher.launcher.Game>()
        readDelgaGames(nativeLauncher, path).forEach { g ->
            games.add(ch.dragondreams.delauncher.launcher.Game.getInstance(Game.getInstance(g))!!)
        }
        return games
    }

    fun isModuleSingleType(type: EngineModule.Type): Boolean {
        return isModuleSingleType(ch.dragondreams.delauncher.launcher.internal.EngineModule.mapTypeInv[type]!!)
    }

    fun getGameProfiles(): List<GameProfile> {
        val profiles = mutableListOf<GameProfile>()
        getGameProfiles(nativeLauncher).forEach { p ->
            profiles.add(GameProfile.getInstance(
                ch.dragondreams.delauncher.launcher.internal.GameProfile.getInstance(p))!!)
        }
        return profiles
    }

    fun getDefaultProfile(): GameProfile? {
        return GameProfile.getInstance(
            ch.dragondreams.delauncher.launcher.internal.GameProfile.getInstance(
                getDefaultProfile(nativeLauncher)))
    }

    fun getActiveProfile(): GameProfile? {
        return GameProfile.getInstance(
            ch.dragondreams.delauncher.launcher.internal.GameProfile.getInstance(
                getActiveProfile(nativeLauncher)))
    }

    fun setActiveProfile(profile: GameProfile?){
        setActiveProfile(nativeLauncher, profile?.nativeProfile?.nativeProfile ?: 0L)
    }

    fun addFileLogger(path: String){
        addFileLogger(nativeLauncher, path)
    }

    fun prepare(){
        prepare(nativeLauncher)
    }

    fun createGame(): Game{
        return Game.getInstance(createGame(nativeLauncher))!!
    }

    fun logInfo(message: String) {
        logInfo(nativeLauncher, message)
    }

    fun logWarn(message: String) {
        logWarn(nativeLauncher, message)
    }

    fun logError(message: String) {
        logError(nativeLauncher, message)
    }
}
