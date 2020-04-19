#!/bin/bash

ADB="adb"
if [ "$ANDROID_HOME" != "" ]; then
        ADB="${ANDROID_HOME}/platform-tools/adb"
fi

pid="$1"
if [ "$pid" == "" ]; then
	echo "./adbgdbconnect.sh pid"
	exit 1
fi

${ADB} -s 10cc9b60 forward tcp:5039 localfilesystem:/data/data/ch.rptd.dragengine.delauncherandroid/debug-pipe
${ADB} -s 10cc9b60 shell run-as ch.rptd.dragengine.delauncherandroid /data/data/ch.rptd.dragengine.delauncherandroid/lib/gdbserver.so +debug-pipe --attach $pid

# end of script
