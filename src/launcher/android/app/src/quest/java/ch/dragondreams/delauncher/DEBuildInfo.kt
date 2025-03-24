package ch.dragondreams.delauncher

import ch.dragondreams.delauncher.ui.example.Example

class DEBuildInfo {
    companion object {
        private const val EXAMPLES_BASE_URL =
            "https://github.com/LordOfDragons/deexamples/releases/latest/download"

        const val arch = "armv8"
        const val platform = "quest"

        val examples = listOf(
            Example(
                "VR Example",
                "$EXAMPLES_BASE_URL/ExampleVR.delga",
                "0111f016-a31a-134a-4498-567d86bb75ce"
            )
        )
    }
}
