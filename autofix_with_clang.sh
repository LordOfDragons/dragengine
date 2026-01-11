#!/bin/bash

FIX_DIR="./.tidy_fixes"

rm -rf $FIX_DIR && mkdir $FIX_DIR

# ensure problematic flags are removed from the database first
sed -i 's/-fsanitize=bounds-strict//g' compile_commands.json

run-clang-tidy -p . \
  -config-file=.clang-tidy \
  -header-filter=".*/src/.*" \
  -export-fixes=$FIX_DIR \
  "src/.*" -- -fix-errors

# Define patterns to exclude
EXCLUDE_PATTERNS=("build/"
                  "src/dragengine/base64/"
                  "src/dragengine/src/extern/"
                  "src/deigde/editors/project/minizip/"
                  "src/modules/archive/delga/minizip"
                  "src/modules/graphic/opengl/src/convexhull3d/"
                  "src/modules/graphic/opengl/src/extensions/androidfix"
                  "src/modules/graphic/opengl/src/extensions/egl"
                  "src/modules/graphic/opengl/src/extensions/gl"
                  "src/modules/graphic/opengl/src/extensions/khr"
                  "src/modules/graphic/opengl/src/extensions/macosfix"
                  "src/modules/graphic/opengl/src/extensions/wasmfix"
                  "src/modules/graphic/opengl/src/extensions/wgl"
                  "src/modules/graphic/opengl/squish/"
                  "src/modules/physics/bullet/Bullet/"
                  "src/modules/vr/openxr/src/loader/"
                  "src/modules/vr/openxr/src/extension/xdev/"
                  "src/modules/vr/openxr/src/graphicapi/deoxrGraphicApiOpenGL.cpp"
                  "src/shared/vulkan/extern/include/")

echo "Cleaning $FIX_DIR..."

for file in "$FIX_DIR"/*.yaml; do
    [ -e "$file" ] || continue

    for pattern in "${EXCLUDE_PATTERNS[@]}"; do
        # Use grep -F for literal string matching and -i for case-insensitivity
        # We search specifically for the 'FilePath:' key followed by the pattern
        if grep -qi "FilePath:.*$pattern" "$file"; then
            echo "Deleting $file (contains excluded path: $pattern)"
            rm "$file"
            break
        fi
    done
done

echo "Clean up complete."

# apply the fixes from the YAML files to the actual source
# Use the . (current directory) as the search root for your source files
clang-apply-replacements --remove-change-desc-files $FIX_DIR
