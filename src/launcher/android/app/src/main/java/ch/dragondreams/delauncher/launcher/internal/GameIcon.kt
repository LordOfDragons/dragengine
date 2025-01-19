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

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as GameIcon

        if (size != other.size) return false
        if (path != other.path) return false
        if (!content.contentEquals(other.content)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = size
        result = 31 * result + path.hashCode()
        result = 31 * result + content.contentHashCode()
        return result
    }
}
