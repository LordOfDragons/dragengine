package ch.dragondreams.delauncher

import android.app.Activity
import android.content.DialogInterface
import android.content.Intent
import android.net.Uri
import android.view.Window
import androidx.appcompat.app.AlertDialog
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat

class UIHelper {
    companion object{
        fun openUrl(activity: Activity, url: String){
            val webpage = Uri.parse(url)
            val intent = Intent(Intent.ACTION_VIEW, webpage)
            try{
                activity.startActivity(intent)
            }catch (e: Exception){
                UIHelper.showError(activity, R.string.error_failed_open_url)
            }
        }

        fun showError(activity: Activity, messageId: Int){
            showError(activity, messageId, R.string.title_error)
        }

        fun showError(activity: Activity, messageId: Int, titleId: Int){
            showError(activity, activity.getString(messageId), activity.getString(titleId))
        }

        fun showError(activity: Activity, message: String){
            showError(activity, message, activity.getString(R.string.title_error))
        }

        fun showError(activity: Activity, message: String, title: String){
            val builder = AlertDialog.Builder(activity)
            builder.setMessage(message)
            builder.setTitle(title)
            builder.setCancelable(true) // can close clicking outside dialog
            builder.setPositiveButton(R.string.label_close) { dialog: DialogInterface?, _: Int ->
                dialog?.cancel()
            }
            builder.create().show()
        }

        fun confirm(activity: Activity, messageId: Int, titleId: Int,
                    labelAccept: Int, labelReject: Int,
                    code: (accepted: Boolean) -> Unit){
            confirm(activity, activity.getString(messageId), activity.getString(titleId),
                labelAccept, labelReject, code)
        }

        fun confirm(activity: Activity, message: String, title: String,
                    labelAccept: Int, labelReject: Int,
                    code: (accepted: Boolean) -> Unit){
            val builder = AlertDialog.Builder(activity)
            builder.setMessage(message)
            builder.setTitle(title)
            builder.setCancelable(false) // can not close clicking outside dialog
            builder.setPositiveButton(labelAccept) { dialog: DialogInterface?, _: Int ->
                dialog?.cancel()
                code(true)
            }
            builder.setNegativeButton(labelReject) { dialog: DialogInterface?, _: Int ->
                dialog?.cancel()
                code(false)
            }
            builder.create().show()
        }

        fun enableSystemUIBars(window: Window, enable: Boolean) {
            /*
            if (enable) {
                //window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN)
                window.decorView.systemUiVisibility -= (
                        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                or View.SYSTEM_UI_FLAG_FULLSCREEN)
            } else {
                //window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN)
                window.decorView.systemUiVisibility += (
                        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                or View.SYSTEM_UI_FLAG_FULLSCREEN)
            }
            */

            val wic = WindowCompat.getInsetsController(window, window.decorView)
            if (enable) {
                wic.systemBarsBehavior = WindowInsetsControllerCompat.BEHAVIOR_DEFAULT
                wic.show(WindowInsetsCompat.Type.systemBars())

            } else {
                wic.systemBarsBehavior = WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
                wic.hide(WindowInsetsCompat.Type.systemBars())
            }
        }
    }
}
