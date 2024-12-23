#!/bin/bash

# copy libraries
TARGET_DIR=app/src/main/jniLibs/arm64-v8a
mkdir -p $TARGET_DIR
cp ../../dragengine/build/libdragengine.so $TARGET_DIR
cp ../shared/build/libdelauncher.so $TARGET_DIR

# copy assets
TARGET_DIR=app/src/main/assets/dragengine
SOURCE_DIR=../../../build/release
rm -rf $TARGET_DIR
mkdir -p $TARGET_DIR
cp -r $SOURCE_DIR/{lib,share} $TARGET_DIR

# delete shared libraries copied to assets directory
rm -f $TARGET_DIR/lib/lib*.so*

# delete other files copied that are not required
rm -rf $TARGET_DIR/share/deigde
rm -rf $TARGET_DIR/share/katepart5
rm -rf $TARGET_DIR/share/mime

# zip asset
( cd $TARGET_DIR && zip -r ../dragengine.zip * )
rm -rf $TARGET_DIR
