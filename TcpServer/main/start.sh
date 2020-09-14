#!/bin/sh

export LD_LIBRARY_PATH=../lib/

case "$1" in
	start)
		./a.out;;
	stop)
		killall a.out;;
esac


