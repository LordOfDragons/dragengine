#!/bin/bash
( cd ../../../.. && scons -j 1 all $* ) && \
   ./pack_assets.sh && \
   ( cd DEAndroidLauncher && ./gradlew assembleDebug )
