package ch.dragondreams.delauncher

import android.os.Bundle
import android.util.Log
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.net.toFile
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import java.io.File

class RunDelgaActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_run_delga)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        Log.i(TAG, listOf("processIntentLaunchDelga:",
            "action='${intent.action}'",
            "type='${intent.type}'",
            "scheme='${intent.scheme}'",
            "data='${intent.data}'").joinToString(" "))

        if (intent.action == "ch.dragondreams.delauncher.LAUNCH_DELGA"
            || intent.action == "android.intent.action.VIEW") {
            processIntentLaunchDelga()

        } else {
            finish()
        }
    }

    private fun processIntentLaunchDelga() {
        // we can not use this check since google file app sends ACTION_VIEW intents
        // with type "application/octet-stream" and the URI path obfuscated to stuff like
        // "com.google.android.apps.nbu.files.provider/2/1000000579". this hides the file
        // pattern and also the filename. lucky us though we do not need the correct
        // filename for launching to work. so much for overzealous security ideas
        /*
        if (intent.dataString?.endsWith(".delga") != true) {
            finish()
            return
        }
        */

        when (intent.data?.scheme) {
            "content" -> {
                contentResolver.openFileDescriptor(intent.data!!, "r").use { pfd ->
                    val fd = pfd?.fd
                    Log.i(TAG, "processIntentLaunchDelga: file descriptor $fd")
                }
            }

            "file" -> {
                val path = intent.data?.toFile()
                if (path == null) {
                    finish()
                    return
                }

                Log.i(TAG, "processIntentLaunchDelga: $path")
                path.inputStream().use { s ->
                    Log.i(TAG, "processIntentLaunchDelga: stream $s")
                }
            }

            else -> {
                finish()
            }
        }


    }

    companion object {
        const val TAG = "RunDelgaActivity"
    }
}
