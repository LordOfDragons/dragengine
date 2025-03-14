package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.DragengineLauncher

class RemoteLauncherHandler(
    val client: RemoteLauncherClient,
    val launcher: DragengineLauncher,
    game: ch.dragondreams.delauncher.launcher.Game,
    runParams: ch.dragondreams.delauncher.launcher.GameRunParams,
    listener: RunGameHandler.Listener
) {
    val nativeHandler: Long = createHandler(
        client.nativeClient,
        launcher.launcher!!.nativeLauncher,
        game.nativeGame.nativeGame,
        runParams.toNative(),
        listener)

    val game: ch.dragondreams.delauncher.launcher.Game = game.retain()

    private external fun createHandler(client: Long, launcher: Long, game: Long,
                                       params: GameRunParams,
                                       listener: RunGameHandler.Listener): Long
    private external fun destroyHandler(handler: Long)
    private external fun getState(handler: Long): Int
    private external fun waitForState(handler: Long, state: Int)
    private external fun stopGame(handler: Long)

    fun dispose(){
        destroyHandler(nativeHandler)
    }

    fun getState(): RunGameHandler.State{
        return RunGameHandler.mapState[getState(nativeHandler)]!!
    }

    fun waitForState(state: RunGameHandler.State){
        waitForState(nativeHandler, RunGameHandler.mapStateRev[state]!!)
    }

    fun stopGame(){
        stopGame(nativeHandler)
    }
}
