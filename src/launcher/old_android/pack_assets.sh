#!/bin/bash

dir_x86=x86
dir_arm=armeabi
dir_armv7a=armeabi-v7a

( cd ../../../modules/scripting/dragonscript/testproject
  rm -f ../../../../../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/dstestproject.zip || exit 1
  echo "Creating dstestproject.zip..."
  zip -q -r ../../../../../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/dstestproject.zip data dstestproject.degame.xml || exit 1
  ( cd ../../../../../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets
    sha1sum dstestproject.zip > dstestproject.sha1sum ) )

( cd ../../../../build_android
  if [ -d ${dir_x86}/lib ]; then
    rm -f ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/install_${dir_x86}.zip || exit 1
    echo "Creating install_${dir_x86}.zip..."
    zip -q -r ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/install_${dir_x86}.zip \
      ${dir_x86}/lib ${dir_x86}/share || exit 1
    ( cd ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets
      sha1sum install_${dir_x86}.zip > install_${dir_x86}.sha1sum )
  fi

  if [ -d ${dir_arm}/lib ]; then
    rm -f ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/install_${dir_arm}.zip || exit 1
    echo "Creating install_${dir_arm}.zip..."
    zip -q -r ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/install_${dir_arm}.zip \
      ${dir_arm}/lib ${dir_arm}/share || exit 1
    ( cd ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets
      sha1sum install_${dir_arm}.zip > install_${dir_arm}.sha1sum )
  fi

  if [ -d ${dir_armv7a}/lib ]; then
    rm -f ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/install_${dir_armv7a}.zip || exit 1
    echo "Creating install_${dir_armv7a}.zip..."
    zip -q -r ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets/install_${dir_armv7a}.zip \
      ${dir_armv7a}/lib ${dir_armv7a}/share || exit 1
    ( cd ../src/tools/launcher/android/DEAndroidLauncher/app/src/main/assets
      sha1sum install_${dir_armv7a}.zip > install_${dir_armv7a}.sha1sum )
  fi
)

# end of script

