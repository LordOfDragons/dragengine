#!/bin/bash

# IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
#
# the make-standalone-toolchain.sh file in NDK is bugged for hard floating point toolchain
# armva-hard. the file has to be fixed or STL won't compile.
#
# find this method definition in the file:
#   copy_stl_libs_for_abi () {
#
# locate in the there case
#   armeabi-v7a-hard)
#
# replace the two lines in there with these two lines
#            copy_stl_libs armeabi-v7a-hard "armv7-a/hard/bits"   "bits"       "armv7-a/hard"       "."
#            copy_stl_libs armeabi-v7a-hard "armv7-a/thumb/hard/bits"  "bits"  "armv7-a/thumb/hard" "."
#
# now the toolchains can be build

export NDK=/opt/android-ndk
export INSTALLDIR=/opt/android-ndk-toolchains

rm -r toolchain_arm toolchain_x86
mkdir toolchain_arm toolchain_x86

apilevel=18
#apilevel=21

hostarch=linux-x86_64

# supported gcc versions are 4.8 and 4.9
gccversion=4.9

# apilevel 18:
#   android 4.3.x JellyBean
#   supports EGL 3.0
#   minimum version required to use EGL 3.0 before is EGL 2.0

# apilevel 19:
#   android 4.4.x KitKat
#   supports EGL 3.0
#   last version with strtof and other calls as inline calls. if compiled with higher level loading the library fails
#   because the header contains strtof but the system library not (huge major league fuckup)

# apilevel 21:
#   android 5.0.x Lollipop
#   supports EGL 3.1
#   has strtof in libc. if running on older version loading fails due to missing symbols

# options:
# --toolchain=x86-4.8                    # select x86 GCC 4.8 compiler
# --toolchain=arm-linux-androideabi-4.7  # select ARM GCC 4.7 compiler
# --toolchain=mipsel-linux-android-4.6   # select MIPS GCC 4.6 compiler, same as --arch=mips

${NDK}/build/tools/make-standalone-toolchain.sh \
	--platform=android-${apilevel} \
	--toolchain=arm-linux-androideabi-${gccversion} \
	--abis=armeabi-v7a-hard \
	--install-dir="${INSTALLDIR}/toolchain_arm"
#	--system=${hostarch} \

${NDK}/build/tools/make-standalone-toolchain.sh \
	--platform=android-${apilevel} \
	--toolchain=x86-${gccversion} \
	--install-dir="${INSTALLDIR}/toolchain_x86"
#	--system=${hostarch} \

# end of script
