package ch.dragondreams.delauncher.launcher.internal

import android.view.SurfaceView

data class LauncherConfig(
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