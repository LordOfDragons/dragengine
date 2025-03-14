package ch.dragondreams.delauncher.launcher.internal

data class GameProfileModule(
    var name: String = "",
    var parameters: Array<GameProfileModuleParameter> = emptyArray()
) {
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as GameProfileModule

        if (name != other.name) return false
        if (!parameters.contentEquals(other.parameters)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = name.hashCode()
        result = 31 * result + parameters.contentHashCode()
        return result
    }
}
