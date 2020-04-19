#!/bin/bash
#

for f in `find genpatch -type f`; do
	f2=`echo $f | sed -e "s/^genpatch/Bullet/g"`
	#echo $f $f2
	diff -rupN $f $f2
done

