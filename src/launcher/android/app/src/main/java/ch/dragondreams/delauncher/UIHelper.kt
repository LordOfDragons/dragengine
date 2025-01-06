package ch.dragondreams.delauncher

import android.app.Activity
import android.content.DialogInterface
import android.content.Intent
import android.net.Uri
import androidx.appcompat.app.AlertDialog

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
            val builder = AlertDialog.Builder(activity)
            builder.setMessage(activity.getString(R.string.error_failed_open_url))
            builder.setTitle(activity.getString(R.string.title_error))
            builder.setCancelable(true) // can close clicking outside dialog
            builder.setPositiveButton(R.string.label_close) { dialog: DialogInterface?, _: Int ->
                dialog?.cancel()
            }
            builder.create().show()
        }
    }
}
