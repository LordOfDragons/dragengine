package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.DragengineLauncher

class RemoteLauncherHandler(
    val launcher: DragengineLauncher,
    game: ch.dragondreams.delauncher.launcher.Game,
    val runParams: ch.dragondreams.delauncher.launcher.GameRunParams
){
    val nativeHandler: Long = createHandler(
        launcher.launcher!!.nativeLauncher,
        game.nativeGame.nativeGame,
        runParams.toNative())

    private external fun createHandler(launcher: Long, game: Long, params: GameRunParams): Long
    private external fun destroyHandler(handler: Long)

    fun dispose(){
        destroyHandler(nativeHandler)
    }
}
