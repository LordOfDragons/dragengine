package ch.dragondreams.delauncher.ui.remote

import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.text.method.ScrollingMovementMethod
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.core.widget.doAfterTextChanged
import androidx.fragment.app.Fragment
import androidx.preference.PreferenceManager
import ch.dragondreams.delauncher.PermissionRequestor
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.RunGameShared
import ch.dragondreams.delauncher.UIHelper
import ch.dragondreams.delauncher.launcher.DragengineLauncher
import ch.dragondreams.delauncher.launcher.internal.GameActivityAdapter
import ch.dragondreams.delauncher.launcher.internal.RemoteLauncherClient
import ch.dragondreams.delauncher.launcher.internal.RemoteLauncherClientRunParameters
import ch.dragondreams.delauncher.launcher.internal.RemoteLauncherHandler
import ch.dragondreams.delauncher.launcher.internal.RunGameHandler
import java.io.File
import java.time.Instant
import java.time.ZoneId
import java.time.format.DateTimeFormatter
import java.util.Locale
import kotlin.concurrent.Volatile


class FragmentRemoteLauncher(
    private val permissionRequestor: PermissionRequestor
) : Fragment(), PermissionRequestor.Listener {
    class ClientListener(
        private val owner: FragmentRemoteLauncher
    ) : RemoteLauncherClient.Listener{
        private val handler = Handler(Looper.getMainLooper())

        override fun addLogs(severity: Int, logs: String) {
            owner.addLogs( RemoteLauncherClient.mapLogSeverity[severity]!!, logs)
        }

        override fun updateUIState() {
            // event can be possibly send from UI thread. in this case the block is run directly
            // causing FragmentRemoteLauncher.updateUIState() to call
            // RemoteLauncherClient.isDisconnected() which tries to lock an already locked mutex.
            // to avoid this make sure the event is send delayed in all cases
            if(!owner.isDestroyingView) {
                handler.post { owner.updateUIState() }
            }
        }

        override fun requestProfileNames() {
            if(!owner.isDestroyingView) {
                handler.post { owner.requestProfileNames() }
            }
        }

        override fun requestDefaultProfileName() {
            if(!owner.isDestroyingView) {
                handler.post { owner.requestDefaultProfileName() }
            }
        }

        override fun startApplication(params: RemoteLauncherClientRunParameters) {
            if(!owner.isDestroyingView) {
                handler.post { owner.startApplication(params) }
            }
        }

        override fun stopApplication() {
            if(!owner.isDestroyingView) {
                handler.post { owner.stopApplication() }
            }
        }

        override fun killApplication() {
            if(!owner.isDestroyingView) {
                handler.post { owner.killApplication() }
            }
        }
    }

    class HandlerListener(
        private val owner: FragmentRemoteLauncher
    ) : RunGameHandler.Listener{
        private val handler = Handler(Looper.getMainLooper())

        override fun stateChanged(state: Int) {
            if(!owner.isDestroyingView) {
                val hstate = RunGameHandler.mapState[state]!!

                if(hstate == RunGameHandler.State.GameRunning) {
                    owner.client?.setRunStatus(RemoteLauncherClient.RunStatus.Running)
                }else{
                    owner.client?.setRunStatus(RemoteLauncherClient.RunStatus.Stopped)
                }

                handler.post { owner.handlerStateChanged(hstate) }
            }
        }
    }

    var engineReady = false
        set(value) {
            if(value == field){
                return
            }
            field = value

            if(value){
                client = RemoteLauncherClient(ClientListener(this))
                client?.setLauncher(shared?.launcher)
            }else{
                client?.dispose()
                client = null
            }

            requireActivity().runOnUiThread { updateUIState() }
        }

    var shared: RunGameShared? = null

    private var client: RemoteLauncherClient? = null
    var pathDataDir: String? = null
    private var remoteLauncherHandler: RemoteLauncherHandler? = null
    @Volatile
    private var isDestroyingView = false

    private var editHostAddress: TextView? = null
    private var editClientName: TextView? = null
    private var editLogs: TextView? = null
    private var buttonConnect: Button? = null
    private var buttonDisconnect: Button? = null

    private val logLines = ArrayDeque<String>()
    @Volatile
    private var pendingUpdateEditLogs = false
    private val maxLogLineCount = 100
    private val logTimeFormatter = DateTimeFormatter
        .ofPattern("yyyy-MM-dd HH:mm:ss.SSS")
        .withLocale(Locale.getDefault())
        .withZone(ZoneId.systemDefault())

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_remote_launcher, container, false)

        // settings
        //pathDataDir = File(requireContext().cacheDir, "remotelauncher").absolutePath
        pathDataDir = File(requireContext().filesDir, "remotelauncher").absolutePath

        // find widgets
        editHostAddress = view.findViewById(R.id.editHostAddress)
        editClientName = view.findViewById(R.id.editClientName)
        editLogs = view.findViewById(R.id.textLogs)
        buttonConnect = view.findViewById(R.id.buttonConnect)
        buttonDisconnect = view.findViewById(R.id.buttonDisconnect)

        // set initial states
        val pref = PreferenceManager.getDefaultSharedPreferences(requireContext())
        editHostAddress?.text = pref.getString(PREF_HOST, "")
        editClientName?.text = pref.getString(PREF_CLIENT_NAME, "Android")
        updateUIState()

        // add event listeners
        editHostAddress?.doAfterTextChanged { text ->
            val edit = pref.edit()
            edit.putString(PREF_HOST, text.toString())
            edit.apply()
        }

        editClientName?.doAfterTextChanged { text ->
            val edit = pref.edit()
            edit.putString(PREF_CLIENT_NAME, text.toString())
            edit.apply()
        }

        editLogs?.movementMethod = ScrollingMovementMethod()

        buttonConnect?.setOnClickListener { onButtonConnect() }
        buttonDisconnect?.setOnClickListener { onButtonDisconnect() }

        permissionRequestor.addListener(REQUEST_CODE_PERMISSION_INTERNET, this)
        return view
    }

    override fun onDestroyView() {
        permissionRequestor.removeListener(REQUEST_CODE_PERMISSION_INTERNET, this)
        isDestroyingView = true
        stopApplicationWait()

        GameActivityAdapter().setHandler(0L)
        remoteLauncherHandler?.dispose()
        remoteLauncherHandler = null

        shared?.dispose()
        shared = null

        client?.dispose()
        client = null

        super.onDestroyView()
    }

    private fun onButtonConnect() {
        permissionRequestor.requiresPermissionInternet(requireActivity(),
            REQUEST_CODE_PERMISSION_INTERNET,
            R.string.message_requires_permission_internet_remote) {
                connectToHost()
        }
    }

    private fun connectToHost(){
        val clientName = editClientName?.text?.toString() ?: ""
        if(clientName.isEmpty()){
            requireActivity().runOnUiThread {
                UIHelper.showError(requireActivity(), R.string.error_client_name_empty)
            }
            return
        }

        val pathDataDir = pathDataDir ?: ""
        if(pathDataDir.isEmpty()){
            requireActivity().runOnUiThread {
                UIHelper.showError(requireActivity(), R.string.error_data_path_empty)
            }
            return
        }

        val address = editHostAddress?.text?.toString() ?: ""
        if(address.isEmpty()){
            requireActivity().runOnUiThread {
                UIHelper.showError(requireActivity(), R.string.error_host_address_empty)
            }
            return
        }

        try{
            client?.connectToHost(clientName, pathDataDir, address)
        }catch(e: Exception){
            shared?.logError("onButtonConnect", "Connect failed", e)
            requireActivity().runOnUiThread {
                UIHelper.showError(requireActivity(), R.string.error_connect_failed)
            }
        }

        requireActivity().runOnUiThread {
            updateUIState()
        }
    }

    private fun onButtonDisconnect(){
        try{
            client?.disconnectFromHost()
        }catch(e: Exception){
            shared?.logError("onButtonConnect", "Connect failed", e)
            requireActivity().runOnUiThread {
                UIHelper.showError(requireActivity(), R.string.error_disconnect_failed)
            }
        }

        requireActivity().runOnUiThread {
            updateUIState()
        }
    }

    fun updateUIState(){
        if(client?.isDisconnected() != false){ // disconnected or client is null
            editClientName?.isEnabled = true
            editHostAddress?.isEnabled = true
            buttonConnect?.isEnabled = true
            buttonDisconnect?.isEnabled = false
            return
        }

        editClientName?.isEnabled = false
        editHostAddress?.isEnabled = false
        buttonConnect?.isEnabled = false
        buttonDisconnect?.isEnabled = true

        updateFragmentVisiblity()
    }

    private fun updateFragmentVisiblity(){
        val newVisible = remoteLauncherHandler?.getState() != RunGameHandler.State.GameRunning
        if(isVisible == newVisible){
            return
        }

        val t = parentFragmentManager.beginTransaction()
        if(newVisible) {
            t.show(this)
        }else{
            t.hide(this)
        }
        t.commit()
    }

    fun addLogs(severity: RemoteLauncherClient.LogSeverity, logs: String) {
        if(logs.isEmpty()){
            return
        }

        var text = when(severity){
            RemoteLauncherClient.LogSeverity.Debug -> "[DD] "
            RemoteLauncherClient.LogSeverity.Warning -> "[WW] "
            RemoteLauncherClient.LogSeverity.Error -> "[EE] "
            RemoteLauncherClient.LogSeverity.Info -> "[II] "
        }
        text += "["
        text += logTimeFormatter.format(Instant.now())
        text += "] "
        text += logs

        // so not use shared?.logInfo or similar as this can result in a dead-loop
        Log.i(TAG, "Client: $logs")

        synchronized(logLines){
            logLines.addLast(text)
            while(logLines.size > maxLogLineCount) {
                logLines.removeFirst()
            }
        }

        if(!pendingUpdateEditLogs && !isDestroyingView) {
            requireActivity().runOnUiThread { updateEditLogs() }
        }
    }

    fun updateEditLogs() {
        pendingUpdateEditLogs = false
        if(editLogs != null && !isDestroyingView) {
            editLogs?.text = synchronized(logLines) { logLines.joinToString("\n") }

            requireActivity().runOnUiThread {
                val e = editLogs!!
                e.scrollTo(0, e.layout.getLineTop(e.lineCount) - e.height)
            }
        }
    }

    fun onPermissionResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        when(requestCode){
            REQUEST_CODE_PERMISSION_INTERNET -> {
                // If request is cancelled, the result arrays are empty.
                if((grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
                    connectToHost()
                } else {
                    requireActivity().runOnUiThread {
                        UIHelper.showError(requireActivity(),  R.string.message_remote_launch_internet_denied)
                    }
                }
            }
        }
    }

    fun requestProfileNames() {
        val names = buildList {
            shared?.launcher?.gameProfiles?.forEach {
                p -> add(p.name)
            }
        }
        client?.sendSystemProperty(
            RemoteLauncherClient.SystemPropertyNames.PROPERTY_NAMES,
            names.joinToString("\n"))
    }

    fun requestDefaultProfileName() {
        client?.sendSystemProperty(
            RemoteLauncherClient.SystemPropertyNames.DEFAULT_PROFILE,
            shared?.launcher?.activeProfile?.name ?: "")
    }

    fun startApplication(params: RemoteLauncherClientRunParameters) {
        if(shared?.launcher?.state != DragengineLauncher.State.EngineReady){
            requireActivity().runOnUiThread {
                UIHelper.showError(requireActivity(), R.string.message_game_engine_not_ready)
            }
            return
        }

        if(remoteLauncherHandler != null){
            requireActivity().runOnUiThread {
                UIHelper.showError(requireActivity(), R.string.message_game_running)
            }
            return
        }

        val s = shared!!
        s.logInfo("startApplication", "Game Config: ${params.gameConfig}")

        val l = s.launcher!!
        s.game = l.createGame()

        val g = s.game!!
        g.loadStringConfig(params.gameConfig, l)
        g.updateInfo()
        g.updateConfig()

        g.setGameDirectory(pathDataDir!!)
        g.storeConfig()

        if(g.pathConfig.isEmpty()){
            s.logError("startApplication", "No configuration path specified, ignoring game file.")
            return
        }
        if(g.pathCapture.isEmpty()){
            s.logError("startApplication", "No capture path specified, ignoring game file.")
            return
        }

        s.loadGameConfig()

        if(!g.canRun){
            s.logGameProblems()
            return
        }

        val runProfile = l.gameProfiles.find { p -> p.name == params.profileName }
        if(!s.locateProfile(runProfile)){
            return
        }

        if(params.arguments.isNotEmpty()) {
            s.runParams.runArguments += " "
            s.runParams.runArguments += params.arguments
        }

        s.logInfo("startGame", "Cache application ID = '${g.identifier}'")
        s.logInfo("startGame", "Starting game '${g.title}' using profile '${s.runParams.gameProfile?.name}'")

        remoteLauncherHandler = RemoteLauncherHandler(
            client!!, l, g, s.runParams, HandlerListener(this)
        )
        GameActivityAdapter().setHandler(remoteLauncherHandler!!.nativeHandler)

        activity?.runOnUiThread {
            if (activity != null) {
                UIHelper.enableSystemUIBars(requireActivity().window, false)
            }
        }
    }

    fun stopApplication() {
        val s = shared!!
        s.logInfo(TAG, "stopApplication")
        remoteLauncherHandler?.stopGame()
    }

    fun stopApplicationWait() {
        val s = shared!!
        s.logInfo(TAG, "stopApplication")
        remoteLauncherHandler?.stopGame()
        remoteLauncherHandler?.waitForState(RunGameHandler.State.GameStopped)
    }

    fun killApplication() {
        val s = shared!!
        s.logInfo(TAG, "killApplication")
        remoteLauncherHandler?.stopGame()
    }

    private fun handlerStateChanged(state: RunGameHandler.State) {
        if(state == RunGameHandler.State.GameStopped){
            GameActivityAdapter().setHandler(0L)
            remoteLauncherHandler?.dispose()
            remoteLauncherHandler = null

            activity?.runOnUiThread {
                if (activity != null) {
                    UIHelper.enableSystemUIBars(requireActivity().window, true)
                }
            }
        }
    }

    companion object {
        const val TAG = "FragmentRemoteLauncher"

        const val PREF_HOST = "remote_launcher_host"
        const val PREF_CLIENT_NAME = "remote_launcher_clientname"

        val REQUEST_CODE_PERMISSION_INTERNET = PermissionRequestor.nextRequestCode()
    }
}