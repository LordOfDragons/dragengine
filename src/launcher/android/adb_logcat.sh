#!/bin/bash

ADB="adb"
if [ "$ANDROID_HOME" != "" ]; then
  ADB="${ANDROID_HOME}/platform-tools/adb"
fi

LOGCATFILE="../../../../../LOGCAT"
#LOGFILTER="Dragengine"
LOGFILTER="deandroidlauncher"

if [ "$1" == "-all" ]
then
  stdbuf -o 0 ${ADB} -s 10cc9b60 logcat *:V \
    | grep -vF "ThermalEngine:" \
    | grep -vF "SensorService:" \
    | tee ${LOGCATFILE}
else
  stdbuf -o 0 ${ADB} -s 10cc9b60 logcat ${LOGFILTER}:V \
    | grep ${LOGFILTER} \
    | tee ${LOGCATFILE}
fi

# end of script
