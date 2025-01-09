package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.DragengineLauncher

class RunGameHandler(
    val launcher: DragengineLauncher,
    game: ch.dragondreams.delauncher.launcher.Game,
    runParams: ch.dragondreams.delauncher.launcher.GameRunParams
){
    enum class State{
        StartGame,
        GameRunning,
        GameStopped
    };

    val nativeHandler: Long = createHandler(
        launcher.launcher!!.nativeLauncher,
        game.nativeGame.nativeGame,
        runParams.toNative())

    val game: ch.dragondreams.delauncher.launcher.Game = game.retain()

    private external fun createHandler(launcher: Long, game: Long, params: GameRunParams): Long
    private external fun destroyHandler(handler: Long)

    fun dispose(){
        destroyHandler(nativeHandler)
        game.release()
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
