#!/bin/bash

ADB="adb"
if [ "$ANDROID_HOME" != "" ]; then
        ADB="${ANDROID_HOME}/platform-tools/adb"
fi

${ADB} -s 10cc9b60 pull /sdcard/Android/data/ch.rptd.dragengine.deandroidlauncher/files/etc/logs build/

# end of script
