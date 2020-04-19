#!/bin/bash
#

if [ "$1" == "" ]; then
	echo "specify patch file to apply"
	exit 1
fi

( cd Bullet && patch -p1 < ../$1 )
