package ch.dragondreams.delauncher.launcher

class EngineModuleParameterInfo {
    /** Parameter types. */
    enum class Type{
        /** Boolean value ('0' or '1'). */
        Boolean,

        /** Numeric value. */
        Numeric,

        /** Numeric value which has to be inside certain range. */
        Ranged,

        /** String value. */
        String,

        /** String value from selection of values. */
        Selection
    }

    /** Display category. */
    enum class Category {
        /**
         * Basic configuration parameter.
         *
         * Parameter has to be shown to the user in configuration dialogs.
         * Basic parameters control aspects of the game users want to change.
         */
        Basic,

        /**
         * Advanced configuration parameter.
         *
         * Parameter can be shown to the user in configuration dialogs if the user
         * requests to see advanced parameters. Advanced parameters control advanced
         * aspects of the game user way want to change if they are more experienced.
         */
        Advanced,

        /**
         * Expert parameter.
         *
         * Parameter has to be hidden from user in configuration dialogs. Expert
         * parameters should only be configurable through console commands or using
         * configuration files for expert users only.
         */
        Expert
    }

    /** Selection entry. */
    class SelectionEntry{
        var value = ""
        var displayName = ""
        var description = ""
    }

    var type = Type.Numeric
    var name = ""
    var description = ""
    var minValue = 0.0
    var maxValue = 0.0
    var valueStepSize = 0.0
    var selectionEntries: Array<SelectionEntry> = emptyArray()
    var category = Category.Basic
    var displayName = ""
    var defaultValue = ""
}
