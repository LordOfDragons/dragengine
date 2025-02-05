package ch.dragondreams.delauncher.launcher.internal

import android.app.Activity
import android.view.Surface

data class LauncherConfig(
    var loggerSource: String = "",
    var engineLogFileTitle: String = "",
    var pathLauncher: String = "",
    var pathLauncherGames: String = "",
    var pathLauncherConfig: String = "",
    var pathEngine: String = "",
    var pathEngineConfig: String = "",
    var pathEngineCache: String = "",
    var activity: Activity,
    var surface: Surface? = null
)
