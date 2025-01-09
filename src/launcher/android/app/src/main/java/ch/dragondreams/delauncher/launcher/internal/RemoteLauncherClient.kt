package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.DragengineLauncher

class RemoteLauncherClient(
    private val listener: Listener
) {
    enum class LogSeverity{ Error, Warning, Info, Debug }

    interface Listener {
        fun addLogs(severity: Int, logs: String)
        fun updateUIState()
        fun requestProfileNames()
        fun requestDefaultProfileName()
        fun startApplication(params: RemoteLauncherClientRunParameters)
        fun stopApplication()
        fun killApplication()
    }

    val nativeClient: Long = createClient(listener)

    private external fun createClient(listener: Listener): Long
    private external fun destroyClient(client: Long)
    private external fun connectToHost(client: Long, name: String, pathDataDir: String, address: String)
    private external fun disconnectFromHost(client: Long)
    private external fun isDisconnected(client: Long): Boolean
    private external fun setLauncher(client: Long, launcher: Long)
    private external fun sendSystemProperty(client: Long, property: String, value: String)

    fun dispose(){
        destroyClient(nativeClient)
    }

    fun connectToHost(name: String, pathDataDir: String, address: String){
        connectToHost(nativeClient, name, pathDataDir, address)
    }

    fun disconnectFromHost(){
        disconnectFromHost(nativeClient)
    }

    fun isDisconnected(): Boolean{
        return isDisconnected(nativeClient)
    }

    fun setLauncher(launcher: DragengineLauncher?){
        setLauncher(nativeClient, launcher?.launcher?.nativeLauncher ?: 0L)
    }

    fun sendSystemProperty(property: String, value: String){
        sendSystemProperty(nativeClient, property, value)
    }

    /** System property names. */
    class SystemPropertyNames{
        companion object {
            /** List of supported property names as newline separated string. */
            const val PROPERTY_NAMES = "properties.names"

            /** \brief List of available profile names as newline separated string. */
            const val PROFILE_NAMES = "profiles.names"

            /** \brief Default profile name. */
            const val DEFAULT_PROFILE = "profiles.default"
        }
    }

    companion object{
        val mapLogSeverity = mapOf(
            0 to LogSeverity.Error,
            1 to LogSeverity.Warning,
            2 to LogSeverity.Info,
            3 to LogSeverity.Debug)
    }
}
