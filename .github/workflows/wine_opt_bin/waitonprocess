#!/bin/sh

# inspired by http://stackoverflow.com/a/10407912

echo "Start waiting on $@"
while pgrep -u xclient "$@" > /dev/null; do 
		echo "waiting ..."
		sleep 1; 
done
echo "$@ completed"
