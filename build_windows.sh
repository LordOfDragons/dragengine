#!/bin/bash
if [ "$1" == "" ]; then
	echo "Default is install. This is not what you want."
	exit 1
fi
scons tools=mingw64 $*
# end of script
