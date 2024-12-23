#!/bin/bash

ADB="adb"
if [ "$ANDROID_HOME" != "" ]; then
	ADB="${ANDROID_HOME}/platform-tools/adb"
fi

${ADB} -s 10cc9b60 install -r DEAndroidLauncher/app/build/outputs/apk/app-debug.apk

# end of script
