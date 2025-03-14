package ch.dragondreams.delauncher

import android.app.Activity
import android.content.pm.PackageManager
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

class PermissionRequestor {
    interface Interface {
        fun getPermissionRequestor(): PermissionRequestor
    }

    interface Listener {
        fun onRequestPermissionsResult(requestCode: Int,
                                       permissions: Array<out String>,
                                       grantResults: IntArray)
    }

    private val listeners: MutableMap<Int,MutableList<Listener>> = mutableMapOf()

    fun requiresPermissionInternet(activity: Activity, requestCode: Int,
                                   messageId: Int, runCode: () -> Unit){
        when {
            ContextCompat.checkSelfPermission(activity,
                android.Manifest.permission.INTERNET) == PackageManager.PERMISSION_GRANTED -> {
                    runCode()
            }

            ActivityCompat.shouldShowRequestPermissionRationale(
                activity, android.Manifest.permission.INTERNET
            ) -> {
                activity.runOnUiThread {
                    UIHelper.showError(activity, messageId)
                }
            }

            else -> {
                // You can directly ask for the permission.
                // The registered ActivityResultCallback gets the result of this request.
                ActivityCompat.requestPermissions(activity,
                    arrayOf(android.Manifest.permission.INTERNET), requestCode
                )
            }
        }
    }

    fun onRequestPermissionsResult(requestCode: Int,
                                   permissions: Array<out String>,
                                   grantResults: IntArray): Boolean {
        return listeners[requestCode]?.forEach { l ->
            l.onRequestPermissionsResult(requestCode, permissions, grantResults)
        } != null
    }

    fun addListener(requestCode: Int, listener: Listener) {
        listeners[requestCode]?.add(listener) ?: {
            listeners[requestCode] = mutableListOf(listener)
        }
    }

    fun removeListener(requestCode: Int, listener: Listener) {
        listeners[requestCode]?.remove(listener)
        if (listeners[requestCode]?.isEmpty() == true) {
            listeners.remove(requestCode)
        }
    }

    companion object {
        private var _nextRequestCode = 1000

        fun nextRequestCode(): Int {
            return _nextRequestCode++
        }
    }
}
