#!/bin/bash

ADB="adb"
if [ "$ANDROID_HOME" != "" ]; then
        ADB="${ANDROID_HOME}/platform-tools/adb"
fi

TARGET=/sdcard/Documents/screenrecord.mp4
#BITRATE=4000000
BITRATE=2000000

${ADB} -s 10cc9b60 shell screenrecord --bit-rate ${BITRATE} ${TARGET}

# parameters
# --size 1280×720
# --bit-rate 4000000
#   default is 4Mbits

# end of script
