package ch.dragondreams.delauncher

import ch.dragondreams.delauncher.ui.example.Example

class DEBuildInfo {
    companion object {
        private const val EXAMPLES_BASE_URL =
            "https://github.com/LordOfDragons/deexamples/releases/latest/download"

        const val arch = "armv7"
        const val platform = "android"
        const val runDelgaNoFragment = false

        val examples = listOf(
            Example(
                "UI Example",
                "$EXAMPLES_BASE_URL/DSTestProject.delga",
                "9febef48-f384-4ca2-899e-ae8efdaf996d"
            ),
            Example(
                "3D Example",
                "$EXAMPLES_BASE_URL/DEExampleApp.delga",
                "73ac1719-c9b2-40c4-8cd7-6efa8f5b63f9"
            ),
            Example(
                "Audio Example",
                "$EXAMPLES_BASE_URL/DEAudioTest.delga",
                "355358e9-d3c2-48d3-bbe7-416d45efd9ed"
            )
        )
    }
}
