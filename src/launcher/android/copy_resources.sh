#!/bin/bash

# ensure denetwork and deremotelauncher are present
( cd ../../.. && scons lib_denetwork lib_deremotelauncher )

# copy libraries
TARGET_DIR=app/src/main/jniLibs/arm64-v8a
mkdir -p $TARGET_DIR
cp ../../dragengine/build/libdragengine.so $TARGET_DIR
cp ../shared/build/libdelauncher.so $TARGET_DIR

# for sanitizer copy libraries
#cp ~/Android/Sdk/ndk/27.0.12077973/toolchains/llvm/prebuilt/linux-x86_64/lib/clang/18/lib/linux/libclang_rt.asan-aarch64-android.so $TARGET_DIR

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

du -sb app/src/main/assets/dragengine | cut -f1 >app/src/main/assets/dragengine-filesize

# zip asset
( cd $TARGET_DIR && rm -f ../dragengine.zip && zip -r ../dragengine.zip * )
rm -rf $TARGET_DIR

# store information.
# version can be '1.24-commithash' or 'date' or something unique
#git rev-parse HEAD >app/src/main/assets/dragengine-version
date >app/src/main/assets/dragengine-version
