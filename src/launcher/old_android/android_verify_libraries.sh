#!/bin/bash
READELF="/opt/roland/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf"
$READELF -A ../../../../src/build/libdragengine.so
$READELF -A build/libandroidlauncher_engine.so
$READELF -A build/libandroidlauncher.so
