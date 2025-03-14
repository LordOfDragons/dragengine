package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.DragengineLauncher

class RunGameHandler(
    val launcher: DragengineLauncher,
    game: ch.dragondreams.delauncher.launcher.Game,
    runParams: ch.dragondreams.delauncher.launcher.GameRunParams,
    listener: Listener
){
    enum class State{
        StartGame,
        GameRunning,
        GameStopped
    }

    interface Listener {
        fun stateChanged(state: Int)
    }

    val nativeHandler: Long = createHandler(
        launcher.launcher!!.nativeLauncher,
        game.nativeGame.nativeGame,
        runParams.toNative(),
        listener)

    val game: ch.dragondreams.delauncher.launcher.Game = game.retain()

    private external fun createHandler(launcher: Long, game: Long, params: GameRunParams,
                                       listener: Listener): Long
    private external fun destroyHandler(handler: Long)
    private external fun getState(handler: Long): Int
    private external fun waitForState(handler: Long, state: Int)
    private external fun stopGame(handler: Long)

    fun dispose(){
        destroyHandler(nativeHandler)
        game.release()
    }

    fun getState(): State{
        return mapState[getState(nativeHandler)]!!
    }

    fun waitForState(state: State){
        waitForState(nativeHandler, mapStateRev[state]!!)
    }

    fun stopGame(){
        stopGame(nativeHandler)
    }

    companion object{
        val mapState = mapOf(
            0 to State.StartGame,
            1 to State.GameRunning,
            2 to State.GameStopped
        )

        val mapStateRev = buildMap {
            mapState.forEach { (k, v) -> put(v, k) }
        }
    }
}
