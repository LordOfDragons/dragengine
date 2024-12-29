package ch.dragondreams.delauncher.launcher.internal

data class GameIcon(
    var size: Int = 0,
    var path: String = "",
    var content: ByteArray = byteArrayOf())
{
    fun convert(): ch.dragondreams.delauncher.launcher.GameIcon {
        val icon = ch.dragondreams.delauncher.launcher.GameIcon()
        icon.size = size
        icon.path = path
        icon.content = content
        return icon
    }
}
