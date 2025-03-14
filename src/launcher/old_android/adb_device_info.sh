#!/bin/bash

ADB="adb"
if [ "$ANDROID_HOME" != "" ]; then
        ADB="${ANDROID_HOME}/platform-tools/adb"
fi

${ADB} -s 10cc9b60 shell getprop

# end of script
