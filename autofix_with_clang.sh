#!/bin/bash

FIX_DIR="./.tidy_fixes"

rm -rf $FIX_DIR && mkdir $FIX_DIR

# ensure problematic flags are removed from the database first
sed -i 's/-fsanitize=bounds-strict//g' compile_commands.json

run-clang-tidy -p . \
  -config-file=.clang-tidy \
  -header-filter=".*/src/.*" \
  -checks="-*,modernize-use-override" \
  -export-fixes=$FIX_DIR \
  "src/.*" -- -fix-errors

# Define patterns to exclude
EXCLUDE_PATTERNS=("build/"
                  "src/dragengine/base64"
                  "src/dragengine/src/extern"
                  "src/deigde/editors/project/minizip"
                  "src/modules/archive/delga/minizip"
                  "src/modules/graphic/opengl/src/extensions/"
                  "src/modules/graphic/opengl/squish/"
                  "src/modules/physics/bullet/Bullet/"
                  "src/modules/vr/openxr/src/loader/"
                  "src/modules/vr/openxr/src/extension/xdev/"
                  "src/shared/vulkan/extern/include/")

echo "Cleaning $FIX_DIR..."

for file in "$FIX_DIR"/*.yaml; do
    # Check if the file exists (handles empty directory case)
    [ -e "$file" ] || continue

    for pattern in "${EXCLUDE_PATTERNS[@]}"; do
        # Search for the pattern in the 'FilePath:' line of the YAML
        if grep -q "FilePath:.*$pattern" "$file"; then
            echo "Deleting $file (contains excluded path: $pattern)"
            rm "$file"
            break # Move to next file once deleted
        fi
    done
done

echo "Clean up complete."

# apply the fixes from the YAML files to the actual source
# Use the . (current directory) as the search root for your source files
clang-apply-replacements --remove-change-desc-files $FIX_DIR
