package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.DragengineLauncher

class RemoteLauncherHandler(val launcher: DragengineLauncher){
    val nativeHandler: Long = createHandler(launcher.launcher!!.nativeLauncher)

    private external fun createHandler(launcher: Long): Long
    private external fun destroyHandler(handler: Long)

    fun dispose(){
        destroyHandler(nativeHandler)
    }
}
