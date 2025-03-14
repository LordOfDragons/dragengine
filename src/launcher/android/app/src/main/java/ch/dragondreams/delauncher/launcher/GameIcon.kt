package ch.dragondreams.delauncher.launcher

import android.graphics.Bitmap
import android.graphics.BitmapFactory

class GameIcon {
    var size = 0
    var path = ""
    var content = byteArrayOf()

    private var bitmap: Bitmap? = null
    private var bitmapLoaded = false

    /**
     * Get image loading it if not loaded yet. Can be null if loading image failed
     */
    fun getImage(): Bitmap? {
        if (!bitmapLoaded) {
            bitmap = BitmapFactory.decodeByteArray(content, 0, content.size)
        }
        return bitmap
    }
}
